#include "AdjList.h"
#include "ThreadPool.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <cctype>

/*
    A small HTTP/JSON front end for the airport graph. The graph and the
    precomputed betweenness centrality are loaded once at startup and then only
    read, so a fixed pool of worker threads can answer requests at the same time
    without any locking. Each accepted connection becomes one job handed to the
    pool; a worker reads the request, routes it to the right handler, and writes
    back JSON. The heavy graph work (BFS / Dijkstra) lives in AdjList, not in here.

    Endpoints:
      GET /healthz                                        -> liveness check
      GET /api/v1/airports/{code}                         -> airport metadata
      GET /api/v1/routes?source=..&destination=..&mode=.. -> a route (hops|distance)
      GET /api/v1/network/central-airports?limit=..       -> most central airports
*/

//Sockets look slightly different on Windows and everywhere else, so wrap the few
//pieces we need behind common names.
#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  typedef SOCKET socket_t;
  #define BAD_SOCKET INVALID_SOCKET
  #define closeSocket closesocket
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <sys/time.h>
  #include <unistd.h>
  typedef int socket_t;
  #define BAD_SOCKET (-1)
  #define closeSocket close
#endif

//Everything a worker thread needs to answer a request. Both pointers refer to
//data that is loaded once in main and never changed afterwards.
struct ServerContext {
    AdjList* graph;
    std::vector<std::pair<std::string, double> >* centrality;
};

//The listening socket and a run flag live at file scope so the signal handler can
//reach them. On Ctrl-C we close the listener, which wakes accept() so main can
//fall out of its loop and shut the pool down cleanly.
static volatile sig_atomic_t g_running = 1;
static socket_t g_listen = BAD_SOCKET;

static void onSignal(int) {
    g_running = 0;
    if(g_listen != BAD_SOCKET){
        closeSocket(g_listen);
    }
}

//------------------------------ small helpers --------------------------------

static std::string envOr(const char* key, const std::string& fallback) {
    const char* value = getenv(key);
    return value ? std::string(value) : fallback;
}

//Parse a plain signed integer. Returns false if the whole string isn't one.
static bool parseInt(const std::string& s, int& out) {
    if(s.empty()){
        return false;
    }
    char* endp = nullptr;
    long value = std::strtol(s.c_str(), &endp, 10);
    if(*endp != '\0'){
        return false;
    }
    out = (int)value;
    return true;
}

//Read an integer setting from the environment, clamped to [lo, hi]. A missing
//variable uses the default; a malformed or out-of-range value stops startup, so a
//misconfiguration fails loudly here instead of silently wrapping into nonsense.
static int envInt(const char* key, int fallback, int lo, int hi) {
    const char* raw = getenv(key);
    if(raw == nullptr || raw[0] == '\0'){
        return fallback;
    }
    int value = 0;
    if(!parseInt(std::string(raw), value) || value < lo || value > hi){
        std::cerr << "invalid " << key << ": '" << raw << "' (expected " << lo << ".." << hi << ")\n";
        std::exit(1);
    }
    return value;
}

//Airport codes in the data are upper case, so normalize whatever the client sends.
static std::string toUpper(std::string s) {
    for(unsigned long i = 0; i < s.size(); i++){
        s[i] = (char)toupper((unsigned char)s[i]);
    }
    return s;
}

//Escape the two characters that would otherwise break a JSON string. Our values
//are airport codes and our own messages, so this is all we need.
static std::string jsonEscape(const std::string& s) {
    std::string out;
    for(unsigned long i = 0; i < s.size(); i++){
        if(s[i] == '"' || s[i] == '\\'){
            out += '\\';
        }
        out += s[i];
    }
    return out;
}

//Decode one hex digit, or -1 if it isn't one.
static int hexValue(char c) {
    if(c >= '0' && c <= '9'){ return c - '0'; }
    if(c >= 'a' && c <= 'f'){ return c - 'a' + 10; }
    if(c >= 'A' && c <= 'F'){ return c - 'A' + 10; }
    return -1;
}

//Percent-decode a query value: "%2F" becomes "/", and '+' becomes a space.
static std::string urlDecode(const std::string& s) {
    std::string out;
    for(unsigned long i = 0; i < s.size(); i++){
        if(s[i] == '%' && i + 2 < s.size()){
            int hi = hexValue(s[i + 1]);
            int lo = hexValue(s[i + 2]);
            if(hi >= 0 && lo >= 0){
                out += (char)((hi << 4) | lo);
                i += 2;
                continue;
            }
        }
        if(s[i] == '+'){
            out += ' ';
        } else {
            out += s[i];
        }
    }
    return out;
}

//Pull one value out of an &-separated query string, or "" if the key isn't there.
static std::string queryParam(const std::string& query, const std::string& key) {
    std::string::size_type pos = 0;
    while(pos <= query.size()){
        std::string::size_type amp = query.find('&', pos);
        std::string piece = (amp == std::string::npos) ? query.substr(pos) : query.substr(pos, amp - pos);
        std::string::size_type eq = piece.find('=');
        if(eq != std::string::npos && piece.substr(0, eq) == key){
            return urlDecode(piece.substr(eq + 1));
        }
        if(amp == std::string::npos){
            break;
        }
        pos = amp + 1;
    }
    return "";
}

static std::string statusText(int status) {
    switch(status){
        case 200: return "OK";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 413: return "Payload Too Large";
        case 503: return "Service Unavailable";
        default:  return "Internal Server Error";
    }
}

static std::string jsonError(const std::string& code, const std::string& message) {
    return "{\"error\":{\"code\":\"" + code + "\",\"message\":\"" + jsonEscape(message) + "\"}}";
}

//Turn a path in order into a JSON array of airport codes: ["ORD","SEA","NRT"].
static std::string pathToJson(const std::vector<std::string>& path) {
    std::string out = "[";
    for(unsigned long i = 0; i < path.size(); i++){
        if(i > 0){
            out += ",";
        }
        out += "\"" + jsonEscape(path[i]) + "\"";
    }
    out += "]";
    return out;
}

static std::string numberToString(double value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

//Wrap a status and JSON body in a complete HTTP/1.1 response. We always close the
//connection afterwards, and allow cross-origin reads so the browser client can
//call the API directly.
static std::string httpResponse(int status, const std::string& body) {
    std::ostringstream res;
    res << "HTTP/1.1 " << status << " " << statusText(status) << "\r\n";
    res << "Content-Type: application/json\r\n";
    res << "Content-Length: " << body.size() << "\r\n";
    res << "Access-Control-Allow-Origin: *\r\n";
    res << "Connection: close\r\n";
    res << "\r\n";
    res << body;
    return res.str();
}

static void sendAll(socket_t client, const std::string& data) {
    unsigned long sent = 0;
    while(sent < data.size()){
        int n = send(client, data.data() + sent, (int)(data.size() - sent), 0);
        if(n <= 0){
            break;
        }
        sent += n;
    }
}

//Load the precomputed betweenness centrality from a "CODE: value" file (produced
//offline by main.cpp) and sort it so the most central airports come first.
static bool centralityGreater(const std::pair<std::string, double>& a,
                              const std::pair<std::string, double>& b) {
    return a.second > b.second;
}

static std::vector<std::pair<std::string, double> > loadCentrality(const std::string& file) {
    std::vector<std::pair<std::string, double> > out;
    std::ifstream in(file.c_str());
    if(!in.is_open()){
        std::cerr << "centrality file invalid: " << file << '\n';
        return out;
    }
    std::string code;
    double value;
    while(in >> code >> value){
        //The code is written with a trailing ':', e.g. "PEK:", so trim it off.
        if(!code.empty() && code[code.size() - 1] == ':'){
            code.erase(code.size() - 1);
        }
        out.push_back(std::make_pair(code, value));
    }
    std::sort(out.begin(), out.end(), centralityGreater);
    return out;
}

//------------------------------ endpoint handlers ----------------------------

//GET /api/v1/airports/{code}
static std::pair<int, std::string> handleAirport(ServerContext* ctx, const std::string& code) {
    if(code.empty()){
        return std::make_pair(400, jsonError("MISSING_PARAMETER", "An airport code is required"));
    }
    AdjList::VertexNode* v = ctx->graph->findVertex(code);
    if(v == NULL){
        return std::make_pair(404, jsonError("UNKNOWN_AIRPORT", "Airport code " + code + " was not found"));
    }
    std::string body = "{\"code\":\"" + jsonEscape(v->ID) + "\","
                       "\"latitude\":" + numberToString(v->latitude) + ","
                       "\"longitude\":" + numberToString(v->longitude) + "}";
    return std::make_pair(200, body);
}

//GET /api/v1/routes?source=ORD&destination=NRT&mode=hops|distance
static std::pair<int, std::string> handleRoutes(ServerContext* ctx, const std::string& query) {
    std::string source = toUpper(queryParam(query, "source"));
    std::string destination = toUpper(queryParam(query, "destination"));
    std::string mode = queryParam(query, "mode");
    if(mode.empty()){
        mode = "hops";
    }
    if(source.empty() || destination.empty()){
        return std::make_pair(400, jsonError("MISSING_PARAMETER", "source and destination are required"));
    }
    if(mode != "hops" && mode != "distance"){
        return std::make_pair(400, jsonError("UNSUPPORTED_MODE", "mode must be hops or distance"));
    }
    //Separate an unknown airport (the codes aren't in the graph) from a genuine
    //lack of any route, so the client gets the more specific error.
    if(ctx->graph->findVertex(source) == NULL || ctx->graph->findVertex(destination) == NULL){
        return std::make_pair(404, jsonError("UNKNOWN_AIRPORT", "source or destination was not found"));
    }

    std::ostringstream body;
    if(mode == "hops"){
        std::vector<std::string> route = ctx->graph->BFSPath(source, destination);
        if(route.empty()){
            return std::make_pair(404, jsonError("NO_ROUTE", "No route between " + source + " and " + destination));
        }
        body << "{\"source\":\"" << jsonEscape(source) << "\","
             << "\"destination\":\"" << jsonEscape(destination) << "\","
             << "\"mode\":\"hops\",\"algorithm\":\"bfs\","
             << "\"path\":" << pathToJson(route) << ","
             << "\"hops\":" << (route.size() - 1) << "}";
    } else {
        std::pair<std::vector<std::string>, double> route = ctx->graph->DijkstraPath(source, destination);
        if(route.first.empty()){
            return std::make_pair(404, jsonError("NO_ROUTE", "No route between " + source + " and " + destination));
        }
        body << "{\"source\":\"" << jsonEscape(source) << "\","
             << "\"destination\":\"" << jsonEscape(destination) << "\","
             << "\"mode\":\"distance\",\"algorithm\":\"dijkstra\","
             << "\"path\":" << pathToJson(route.first) << ","
             << "\"hops\":" << (route.first.size() - 1) << ","
             << "\"distanceKm\":" << numberToString(route.second) << "}";
    }
    return std::make_pair(200, body.str());
}

//GET /api/v1/network/central-airports?limit=20
static std::pair<int, std::string> handleCentral(ServerContext* ctx, const std::string& query) {
    int limit = 20;
    std::string limitStr = queryParam(query, "limit");
    if(!limitStr.empty()){
        if(!parseInt(limitStr, limit) || limit < 0){
            return std::make_pair(400, jsonError("INVALID_PARAMETER", "limit must be a non-negative integer"));
        }
    }
    //Keep the response bounded no matter what the client asks for.
    if(limit > 500){
        limit = 500;
    }
    const std::vector<std::pair<std::string, double> >& bc = *(ctx->centrality);
    std::ostringstream body;
    body << "{\"centralAirports\":[";
    for(int i = 0; i < limit && i < (int)bc.size(); i++){
        if(i > 0){
            body << ",";
        }
        body << "{\"code\":\"" << jsonEscape(bc[i].first) << "\","
             << "\"centrality\":" << numberToString(bc[i].second) << "}";
    }
    body << "]}";
    return std::make_pair(200, body.str());
}

//------------------------------ connection handling --------------------------

//Give up on a client that is too slow to send its request, so one stalled
//connection can't tie up a worker thread indefinitely.
static void setReadTimeout(socket_t sock, int seconds) {
#ifdef _WIN32
    DWORD ms = (DWORD)(seconds * 1000);
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&ms, sizeof(ms));
#else
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
#endif
}

//The most request-header bytes we'll read. A GET has no body, so this only needs
//to fit the request line plus a handful of headers; anything larger is rejected.
static const std::string::size_type MAX_REQUEST_BYTES = 16384;

//Read the request headers, up to the blank line that ends them. Sets 'complete'
//once that blank line is seen, and 'tooLarge' if we hit the byte cap before it.
static std::string readRequest(socket_t client, bool& complete, bool& tooLarge) {
    std::string data;
    char buf[4096];
    complete = false;
    tooLarge = false;
    while(true){
        if(data.find("\r\n\r\n") != std::string::npos){
            complete = true;
            break;
        }
        if(data.size() >= MAX_REQUEST_BYTES){
            tooLarge = true;
            break;
        }
        int n = recv(client, buf, sizeof(buf), 0);
        if(n <= 0){
            //Client closed or the read timed out before finishing the headers.
            break;
        }
        data.append(buf, n);
    }
    return data;
}

static void handleConnection(socket_t client, ServerContext* ctx) {
    bool complete = false;
    bool tooLarge = false;
    std::string request = readRequest(client, complete, tooLarge);
    if(tooLarge){
        sendAll(client, httpResponse(413, jsonError("REQUEST_TOO_LARGE", "Request header exceeded the size limit")));
        closeSocket(client);
        return;
    }
    if(!complete){
        sendAll(client, httpResponse(400, jsonError("BAD_REQUEST", "Incomplete request")));
        closeSocket(client);
        return;
    }
    //Parse the request line: METHOD TARGET VERSION.
    std::string firstLine = request.substr(0, request.find("\r\n"));
    std::istringstream iss(firstLine);
    std::string method;
    std::string target;
    iss >> method >> target;

    std::pair<int, std::string> result;
    if(method != "GET"){
        result = std::make_pair(405, jsonError("METHOD_NOT_ALLOWED", "Only GET is supported"));
    } else {
        //Split the target into a path and an optional query string.
        std::string path = target;
        std::string query;
        std::string::size_type qpos = target.find('?');
        if(qpos != std::string::npos){
            path = target.substr(0, qpos);
            query = target.substr(qpos + 1);
        }

        const std::string airportPrefix = "/api/v1/airports/";
        if(path == "/healthz"){
            result = std::make_pair(200, std::string("{\"status\":\"ok\"}"));
        } else if(path == "/readyz"){
            //The graph loads once at startup (and startup aborts if it's empty), so
            //if we're serving at all the graph is ready to answer route queries.
            std::ostringstream ready;
            ready << "{\"status\":\"ready\",\"airports\":" << ctx->graph->vertexList.size()
                  << ",\"routes\":" << ctx->graph->edgeList.size() << "}";
            result = std::make_pair(200, ready.str());
        } else if(path == "/api/v1/routes"){
            result = handleRoutes(ctx, query);
        } else if(path == "/api/v1/network/central-airports"){
            result = handleCentral(ctx, query);
        } else if(path.compare(0, airportPrefix.size(), airportPrefix) == 0){
            result = handleAirport(ctx, toUpper(path.substr(airportPrefix.size())));
        } else {
            result = std::make_pair(404, jsonError("NOT_FOUND", "Unknown endpoint"));
        }
    }

    sendAll(client, httpResponse(result.first, result.second));
    closeSocket(client);
}

//------------------------------ startup --------------------------------------

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    //Configuration comes from the environment so the same binary can serve the
    //500 or 1000 airport set without recompiling. Defaults match the repo data.
    //Hosting platforms (Fly.io, Render, ...) inject the listen port as PORT; fall
    //back to our own AIRPORT_PORT, then the default.
    int port = (getenv("PORT") != nullptr)
                   ? envInt("PORT", 8080, 1, 65535)
                   : envInt("AIRPORT_PORT", 8080, 1, 65535);
    unsigned int threads = (unsigned int)envInt("AIRPORT_THREADS", 4, 1, 256);
    unsigned int maxQueue = (unsigned int)envInt("AIRPORT_MAXQUEUE", 128, 1, 1000000);
    std::string nodesFile = envOr("AIRPORT_NODES", "data/nodes500.txt");
    std::string edgesFile = envOr("AIRPORT_EDGES", "data/edges500.txt");
    std::string bcFile = envOr("AIRPORT_CENTRALITY", "results/Sorted500BC.txt");

    //Load the graph once. From here on it is read-only, so the workers share it.
    AdjList graph(nodesFile, edgesFile);
    std::vector<std::pair<std::string, double> > centrality = loadCentrality(bcFile);

    ServerContext ctx;
    ctx.graph = &graph;
    ctx.centrality = &centrality;

    //Print a short configuration summary so it's clear what the server loaded.
    std::cout << "Airport routing service\n";
    std::cout << "  nodes:      " << nodesFile << " (" << graph.vertexList.size() << " airports)\n";
    std::cout << "  edges:      " << edgesFile << " (" << graph.edgeList.size() << " routes)\n";
    std::cout << "  centrality: " << bcFile << " (" << centrality.size() << " airports)\n";
    std::cout << "  port:       " << port << "\n";
    std::cout << "  threads:    " << threads << "\n";
    std::cout << "  max queue:  " << maxQueue << "\n";

    //Refuse to start on an empty graph: a missing or unreadable dataset should fail
    //loudly here rather than quietly answering 404 for every route.
    if(graph.vertexList.empty()){
        std::cerr << "no airports loaded from " << nodesFile << "; refusing to start\n";
        return 1;
    }

    g_listen = socket(AF_INET, SOCK_STREAM, 0);
    if(g_listen == BAD_SOCKET){
        std::cerr << "socket() failed\n";
        return 1;
    }
    int yes = 1;
    setsockopt(g_listen, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short)port);
    if(bind(g_listen, (sockaddr*)&addr, sizeof(addr)) != 0){
        std::cerr << "bind() failed on port " << port << '\n';
        return 1;
    }
    if(listen(g_listen, 64) != 0){
        std::cerr << "listen() failed\n";
        return 1;
    }

    std::signal(SIGINT, onSignal);
    std::signal(SIGTERM, onSignal);
    std::cout << "listening on http://localhost:" << port << " ...\n" << std::flush;

    {
        //The pool of worker threads. One job is submitted per accepted connection.
        ThreadPool pool(threads, maxQueue);
        while(g_running){
            sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            socket_t client = accept(g_listen, (sockaddr*)&clientAddr, &clientLen);
            if(client == BAD_SOCKET){
                //accept() was interrupted, almost always by shutdown closing the socket.
                break;
            }
            //Don't let a stalled client hold a worker forever.
            setReadTimeout(client, 5);
            ServerContext* cptr = &ctx;
            if(!pool.submit([client, cptr]{ handleConnection(client, cptr); })){
                //Backlog is full: reply with an explicit overload status and move on.
                sendAll(client, httpResponse(503, jsonError("OVERLOADED", "Server is busy, try again shortly")));
                closeSocket(client);
            }
        }
        //Leaving this scope destroys the pool, which drains queued jobs and joins
        //every worker thread before we exit.
    }

    std::cout << "shutting down\n";
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
