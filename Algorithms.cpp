#include "AdjList.h"
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>

double AdjList::getShortestDistance(std::string start, std::string end){
    std::pair<std::string, double> out = FWSingleOutput(start, end);
    return out.second;
}

std::pair<std::string, double> AdjList::FWSingleOutput(std::string start, std::string end){
    if(findVertex(start) == NULL || findVertex(end) == NULL || (start == end))
    {
        std::cout << "Please put in a valid input!" << std::endl;
        return std::pair<std::string, double> ("NULL", 0);
    }
    std::string edge = start + end;
    auto FWMap = FWAlgorithm();
    std::vector<std::string> path;
    AdjList::VertexNode* cur = findVertex(start);
    auto target = findVertex(end);
    bool noPath = false;
    double outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    while(cur != target)
    {
      path.push_back(cur->ID);
      cur = FWMap.find(cur->ID)->second.find(target->ID)->second.second;
      if(cur == NULL){
        path.erase(path.begin());
        noPath = true;
        break;
      }
    }
    if(!noPath)
      path.push_back(cur->ID);
    if(noPath)
    {
        return std::pair<std::string, double> ("NULL", 0);
    }
    std::string out;
    for(unsigned long i = 0; i < path.size(); i++)
    {
        out = out + path[i];
    }


    return std::pair<std::string, double> (out, outDist);
}

std::map<std::string, std::map<std::string, std::pair<double, AdjList::VertexNode*>>> AdjList::FWAlgorithm(){
    //First initialized a double map to represent an Adj matrix. Used strings as keys since cant use a vertex Node as a key.
    //Initialized the main diagonal with zero and the rest with INFINITY. Then went through the edges to change certain values in the adjMatrix
    std::map<std::string, std::map<std::string, std::pair<double, VertexNode*>>> adjMatrix;
    for(std::list<VertexNode*>::iterator vertexItr1 = vertexList.begin(); vertexItr1 != vertexList.end(); vertexItr1++){
        for(std::list<VertexNode*>::iterator vertexItr2 = vertexList.begin(); vertexItr2 != vertexList.end(); vertexItr2++){
            if(*vertexItr1 == *vertexItr2){
                adjMatrix[(*vertexItr1)->ID][(*vertexItr2)->ID].first = 0;
                adjMatrix[(*vertexItr1)->ID][(*vertexItr2)->ID].second= *vertexItr1;
            }
            else{
                adjMatrix[(*vertexItr1)->ID][(*vertexItr2)->ID].first = INFINITY;
            }
        }
    }

    for(std::list<EdgeNode*>::iterator edgeItr = edgeList.begin(); edgeItr != edgeList.end(); edgeItr++){
        adjMatrix[(*edgeItr)->ends.first->ID][(*edgeItr)->ends.second->ID].first = (*edgeItr)->distance;
        adjMatrix[(*edgeItr)->ends.first->ID][(*edgeItr)->ends.second->ID].second = (*edgeItr)->ends.second;
    }

    //Algorithm from the cs225 slides
    for(std::list<VertexNode*>::iterator w = vertexList.begin(); w != vertexList.end(); w++){
        for(std::list<VertexNode*>::iterator u = vertexList.begin(); u != vertexList.end(); u++){
            for(std::list<VertexNode*>::iterator v = vertexList.begin(); v != vertexList.end(); v++){
                if(adjMatrix[(*u)->ID][(*v)->ID].first > adjMatrix[(*u)->ID][(*w)->ID].first + adjMatrix[(*w)->ID][(*v)->ID].first){
                    adjMatrix[(*u)->ID][(*v)->ID].first = adjMatrix[(*u)->ID][(*w)->ID].first + adjMatrix[(*w)->ID][(*v)->ID].first;
                    adjMatrix[(*u)->ID][(*v)->ID].second = adjMatrix[(*u)->ID][(*w)->ID].second;
                }
            }
        }
    }
    return adjMatrix;
}

std::map<std::string, double> AdjList::BCAlgorithm() {
    //loop through each vertex
    std::map<std::string, double> toreturn;
    //contains total number of shortest paths
    std::map<std::string, double> total;
    auto mp = this->FWAlgorithm();
    for (std::list<VertexNode*>::iterator vertexItr1 = vertexList.begin(); vertexItr1 != vertexList.end(); vertexItr1++) {
        //intialize toreuturn map with all vertices
        toreturn.insert(std::make_pair((*vertexItr1)->ID, 0));


    }
    //loop through each map key
    for (auto it = mp.begin(); it != mp.end(); it++) {
        for (auto thing = it->second.begin(); thing != it->second.end(); thing++) {
                //insert first node of path + last node of path
                std::string toinsert = it->first + thing->first;
                auto lookup = total.find(toinsert);
                //if edge already exists, add to existing double value
                if (lookup != total.end()) {
                    lookup->second = lookup->second + 1;
                } else {
                    total.insert(std::make_pair(toinsert, 1));
                }

        }

    }
    double nodes = static_cast<double>(vertexList.size());
    for (std::list<VertexNode*>::iterator vertexItr1 = vertexList.begin(); vertexItr1 != vertexList.end(); vertexItr1++) {
        //loop through each map key
        for (auto it = mp.begin(); it != mp.end(); it++) {
            //loop through nested map
            for (auto nested = it->second.begin(); nested != it->second.end(); nested++) {
                bool check = false;
                auto firstnode = it->first;
                auto lastnode = nested->first;
                //if vertexnode is not at either ends and is in path then check is true
                std::string cur = it->first;
                std::string target = nested->first;
                while (target != cur) {
                    if (mp[cur][target].second != NULL) {
                        cur = mp[cur][target].second->ID;
                    }
                    if (mp[cur][target].second == NULL) {
                        break;
                    }
                    if (cur == (*vertexItr1)->ID) {
                        check = true;
                    }
                    if (check) {
                        break;
                    }
                }
                if ((*vertexItr1)->ID == firstnode || (*vertexItr1)->ID == lastnode) {
                    check = false;
                }
                if (check) {
                    //look up if node is already contained in map
                    auto lookup = toreturn.find((*vertexItr1)->ID);

                    //if node already exists, add to existing double value
                    if (lookup != toreturn.end()) {
                        double num = 0;
                        auto them = total.find(firstnode + lastnode);
                        //search through total shortest path map get total number
                        if (them != total.end()) {
                            num = them->second;
                        }
                        if (num != 0) {
                            lookup->second = lookup->second + 1/num;

                        } else {

                            //do nothing
                        }
                    }
                }

            }
        }
        auto lookup = toreturn.find((*vertexItr1)->ID);

        //if node already exists, add to existing double value
        if (lookup != toreturn.end()) {
            double todivide = ((nodes-1)*(nodes-2));
            if (todivide > 0) {
                lookup->second = lookup->second / todivide;
            }

        }

    }
    return toreturn;
}

//BFS shortest path between two airports. When all we care about is the number of
//connections, the graph is effectively unweighted, so a plain BFS out of the
//start airport reaches every airport in the fewest hops possible. We walk the
//outgoing edges of each airport, remember where we came from, and rebuild the
//path once the destination pops out. Returns the airports in order (start ... end),
//or an empty vector if the destination can't be reached or an airport is unknown.
std::vector<std::string> AdjList::BFSPath(std::string start, std::string end){
    std::vector<std::string> path;
    VertexNode* startVertex = findVertex(start);
    VertexNode* endVertex = findVertex(end);
    //Can't route between airports that aren't in the graph
    if(startVertex == NULL || endVertex == NULL){
        return path;
    }
    //Same airport, so the path is just that airport (zero hops)
    if(start == end){
        path.push_back(start);
        return path;
    }
    //cameFrom remembers which airport we reached each airport from so we can
    //rebuild the path at the end. An airport being in the map also marks it visited.
    std::map<std::string, std::string> cameFrom;
    std::queue<VertexNode*> q;
    q.push(startVertex);
    cameFrom[start] = start;
    bool found = false;
    while(!q.empty() && !found){
        VertexNode* v = q.front();
        q.pop();
        //Edges are stored on their source, so ends.second is always the neighbor.
        //We visit them in insertion order, which keeps the result deterministic
        //when several equal-hop paths exist (first one discovered wins).
        for(std::list<EdgeNode*>::iterator itrEdge = v->edges.begin(); itrEdge != v->edges.end(); itrEdge++){
            VertexNode* w = (*itrEdge)->ends.second;
            if(cameFrom.find(w->ID) == cameFrom.end()){
                cameFrom[w->ID] = v->ID;
                if(w->ID == end){
                    found = true;
                    break;
                }
                q.push(w);
            }
        }
    }
    //If we never reached the destination then there is no route
    if(cameFrom.find(end) == cameFrom.end()){
        return path;
    }
    //Walk backwards from the destination to the start, then flip it around
    std::string cur = end;
    while(cur != start){
        path.push_back(cur);
        cur = cameFrom[cur];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

//Dijkstra's shortest path by distance between two airports. Every edge already
//knows its length in kilometers (set when the edge is inserted), so we run a
//standard Dijkstra out of the start airport: repeatedly expand the closest airport
//we haven't finalized yet and relax its outgoing edges. first is the path in order,
//second is the total distance travelled. An empty path means the destination can't
//be reached or an airport is unknown.
std::pair<std::vector<std::string>, double> AdjList::DijkstraPath(std::string start, std::string end){
    std::vector<std::string> path;
    VertexNode* startVertex = findVertex(start);
    VertexNode* endVertex = findVertex(end);
    if(startVertex == NULL || endVertex == NULL){
        return std::pair<std::vector<std::string>, double>(path, 0);
    }
    if(start == end){
        path.push_back(start);
        return std::pair<std::vector<std::string>, double>(path, 0);
    }
    //Index the airports by ID once up front so that during relaxation we can jump
    //straight from an ID to its node instead of scanning the whole vertex list.
    std::map<std::string, VertexNode*> lookup;
    for(std::list<VertexNode*>::iterator itr = vertexList.begin(); itr != vertexList.end(); itr++){
        lookup[(*itr)->ID] = *itr;
    }
    //dist is the best distance found so far to each airport, cameFrom is the airport
    //we got there from. Everything starts at infinity except the start airport.
    std::map<std::string, double> dist;
    std::map<std::string, std::string> cameFrom;
    for(std::list<VertexNode*>::iterator itr = vertexList.begin(); itr != vertexList.end(); itr++){
        dist[(*itr)->ID] = INFINITY;
    }
    dist[start] = 0;
    //A min-heap keyed on distance, so the top is always the closest airport left.
    //Ties on distance fall back to the airport ID, which keeps results deterministic.
    std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string> >, std::greater<std::pair<double, std::string> > > pq;
    pq.push(std::make_pair(0.0, start));
    while(!pq.empty()){
        double d = pq.top().first;
        std::string uID = pq.top().second;
        pq.pop();
        //A stale entry left behind by an earlier, longer relaxation
        if(d > dist[uID]){
            continue;
        }
        //Once we finalize the destination we can stop early
        if(uID == end){
            break;
        }
        VertexNode* u = lookup[uID];
        for(std::list<EdgeNode*>::iterator itrEdge = u->edges.begin(); itrEdge != u->edges.end(); itrEdge++){
            std::string vID = (*itrEdge)->ends.second->ID;
            double alt = dist[uID] + (*itrEdge)->distance;
            if(alt < dist[vID]){
                dist[vID] = alt;
                cameFrom[vID] = uID;
                pq.push(std::make_pair(alt, vID));
            }
        }
    }
    //Unreachable: no finite distance was ever recorded for the destination
    if(dist[end] == INFINITY){
        return std::pair<std::vector<std::string>, double>(path, 0);
    }
    std::string cur = end;
    while(cur != start){
        path.push_back(cur);
        cur = cameFrom[cur];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return std::pair<std::vector<std::string>, double>(path, dist[end]);
}
