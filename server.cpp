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
#include <cerrno>
#include <climits>
#include <chrono>

/*
    HTTP/JSON front end for the airport graph. Loaded once at startup and then only
    read, so the worker pool shares it without locking. One connection = one job.

      GET /healthz                                        -> liveness
      GET /readyz                                         -> graph loaded
      GET /api/v1/airports/{code}                         -> airport metadata
      GET /api/v1/routes?source=..&destination=..&mode=.. -> route (hops|distance)
      GET /api/v1/network/central-airports?limit=..       -> most central airports
*/

//Winsock and BSD sockets behind common names
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

//Read-only after startup, so every worker shares it
struct ServerContext {
    AdjList* graph;
    std::vector<std::pair<std::string, double> >* centrality;
};

//File scope so onSignal can reach them. Closing the listener wakes accept().
static volatile sig_atomic_t g_running = 1;
static socket_t g_listen = BAD_SOCKET;

//CORS. Set AIRPORT_CORS_ORIGIN to restrict it to one site.
static std::string g_corsOrigin = "*";

static void onSignal(int) {
    g_running = 0;
    if(g_listen != BAD_SOCKET){
        closeSocket(g_listen);
    }
}

static std::string envOr(const char* key, const std::string& fallback) {
    const char* value = getenv(key);
    return value ? std::string(value) : fallback;
}

//whole-string signed integer parse
static bool parseInt(const std::string& s, int& out) {
    if(s.empty()){
        return false;
    }
    errno = 0;
    char* endp = nullptr;
    long value = std::strtol(s.c_str(), &endp, 10);
    //trailing junk, strtol overflow, or too big for int
    if(*endp != '\0' || errno == ERANGE || value < INT_MIN || value > INT_MAX){
        return false;
    }
    out = (int)value;
    return true;
}

//Env int in [lo, hi]. Bad values exit rather than wrap.
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

//airport codes in the data are upper case
static std::string toUpper(std::string s) {
    for(unsigned long i = 0; i < s.size(); i++){
        s[i] = (char)toupper((unsigned char)s[i]);
    }
    return s;
}

//Only codes and our own messages go through here, so these two are enough.
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

//hex digit, or -1
static int hexValue(char c) {
    if(c >= '0' && c <= '9'){ return c - '0'; }
    if(c >= 'a' && c <= 'f'){ return c - 'a' + 10; }
    if(c >= 'A' && c <= 'F'){ return c - 'A' + 10; }
    return -1;
}

//"%2F" -> "/", '+' -> space
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

//one value out of an &-separated query string
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

//["ORD","SEA","NRT"]
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

//Coordinates ride along with the route so the client needs no table of its own.
static std::string coordsToJson(ServerContext* ctx, const std::vector<std::string>& path) {
    std::string out = "[";
    for(unsigned long i = 0; i < path.size(); i++){
        AdjList::VertexNode* v = ctx->graph->findVertex(path[i]);
        if(v == nullptr){
            continue;
        }
        if(out.size() > 1){
            out += ",";
        }
        out += "{\"code\":\"" + jsonEscape(v->ID) + "\","
               "\"lat\":" + numberToString(v->latitude) + ","
               "\"lng\":" + numberToString(v->longitude) + "}";
    }
    out += "]";
    return out;
}

static std::string httpResponse(int status, const std::string& body) {
    std::ostringstream res;
    res << "HTTP/1.1 " << status << " " << statusText(status) << "\r\n";
    res << "Content-Type: application/json\r\n";
    res << "Content-Length: " << body.size() << "\r\n";
    res << "Access-Control-Allow-Origin: " << g_corsOrigin << "\r\n";
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

//"CODE: value" file written offline by main.cpp, most central first
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
        //codes are written "PEK:"
        if(!code.empty() && code[code.size() - 1] == ':'){
            code.erase(code.size() - 1);
        }
        out.push_back(std::make_pair(code, value));
    }
    std::sort(out.begin(), out.end(), centralityGreater);
    return out;
}

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
    //an unknown airport is a more useful error than "no route"
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
             << "\"hops\":" << (route.size() - 1) << ","
             << "\"coordinates\":" << coordsToJson(ctx, route) << "}";
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
             << "\"distanceKm\":" << numberToString(route.second) << ","
             << "\"coordinates\":" << coordsToJson(ctx, route.first) << "}";
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
    //cap it, whatever the client asks for
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

static void setRecvTimeoutMs(socket_t sock, int ms) {
#ifdef _WIN32
    DWORD t = (DWORD)ms;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&t, sizeof(t));
#else
    struct timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
#endif
}

//A GET has no body, so this only needs to fit the request line and headers.
static const std::string::size_type MAX_REQUEST_BYTES = 16384;

//Whole-request deadline, not a per-read gap, so dribbling can't hold a worker.
static const int REQUEST_DEADLINE_MS = 5000;

//Headers up to the blank line. complete = they fit the cap, tooLarge = they didn't.
static std::string readRequest(socket_t client, bool& complete, bool& tooLarge) {
    std::string data;
    char buf[4096];
    complete = false;
    tooLarge = false;
    std::chrono::steady_clock::time_point deadline =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(REQUEST_DEADLINE_MS);
    while(true){
        std::string::size_type marker = data.find("\r\n\r\n");
        if(marker != std::string::npos){
            if(marker + 4 > MAX_REQUEST_BYTES){
                tooLarge = true;
            } else {
                complete = true;
            }
            break;
        }
        if(data.size() >= MAX_REQUEST_BYTES){
            //ran past the cap with no end-of-headers marker
            tooLarge = true;
            break;
        }
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        if(now >= deadline){
            break;
        }
        int remainingMs = (int)std::chrono::duration_cast<std::chrono::milliseconds>(deadline - now).count();
        if(remainingMs < 1){
            remainingMs = 1;
        }
        setRecvTimeoutMs(client, remainingMs);
        std::string::size_type room = (MAX_REQUEST_BYTES + 4) - data.size();
        int want = (int)std::min<std::string::size_type>(sizeof(buf), room);
        int n = recv(client, buf, want, 0);
        if(n <= 0){
            //closed, or recv timed out
            break;
        }
        data.append(buf, (unsigned long)n);
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
    //METHOD TARGET VERSION
    std::string firstLine = request.substr(0, request.find("\r\n"));
    std::istringstream iss(firstLine);
    std::string method;
    std::string target;
    iss >> method >> target;

    std::pair<int, std::string> result;
    if(method != "GET"){
        result = std::make_pair(405, jsonError("METHOD_NOT_ALLOWED", "Only GET is supported"));
    } else {
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
            //startup aborts on an empty graph, so serving at all means ready
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

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
#endif

    //Env config so one binary serves either dataset. Hosts inject PORT.
    int port = (getenv("PORT") != nullptr)
                   ? envInt("PORT", 8080, 1, 65535)
                   : envInt("AIRPORT_PORT", 8080, 1, 65535);
    unsigned int threads = (unsigned int)envInt("AIRPORT_THREADS", 4, 1, 256);
    unsigned int maxQueue = (unsigned int)envInt("AIRPORT_MAXQUEUE", 128, 1, 1000000);
    std::string nodesFile = envOr("AIRPORT_NODES", "data/nodes500.txt");
    std::string edgesFile = envOr("AIRPORT_EDGES", "data/edges500.txt");
    std::string bcFile = envOr("AIRPORT_CENTRALITY", "results/Sorted500BC.txt");
    g_corsOrigin = envOr("AIRPORT_CORS_ORIGIN", "*");

    //read-only from here on, so the workers share it
    AdjList graph(nodesFile, edgesFile);
    std::vector<std::pair<std::string, double> > centrality = loadCentrality(bcFile);

    ServerContext ctx;
    ctx.graph = &graph;
    ctx.centrality = &centrality;

    std::cout << "Airport routing service\n";
    std::cout << "  nodes:      " << nodesFile << " (" << graph.vertexList.size() << " airports)\n";
    std::cout << "  edges:      " << edgesFile << " (" << graph.edgeList.size() << " routes)\n";
    std::cout << "  centrality: " << bcFile << " (" << centrality.size() << " airports)\n";
    std::cout << "  port:       " << port << "\n";
    std::cout << "  threads:    " << threads << "\n";
    std::cout << "  max queue:  " << maxQueue << "\n";

    //a missing dataset should fail here, not quietly serve empty results
    if(graph.vertexList.empty()){
        std::cerr << "no airports loaded from " << nodesFile << "; refusing to start\n";
        return 1;
    }
    if(graph.edgeList.empty()){
        std::cerr << "no routes loaded from " << edgesFile << "; refusing to start\n";
        return 1;
    }
    if(centrality.empty()){
        std::cerr << "no centrality loaded from " << bcFile << "; refusing to start\n";
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
        ThreadPool pool(threads, maxQueue);
        while(g_running){
            sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            socket_t client = accept(g_listen, (sockaddr*)&clientAddr, &clientLen);
            if(client == BAD_SOCKET){
                //interrupted, almost always by shutdown closing the socket
                break;
            }
            ServerContext* cptr = &ctx;
            if(!pool.submit([client, cptr]{ handleConnection(client, cptr); })){
                sendAll(client, httpResponse(503, jsonError("OVERLOADED", "Server is busy, try again shortly")));
                closeSocket(client);
            }
        }
        //~ThreadPool drains the queue and joins every worker
    }

    std::cout << "shutting down\n";
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
