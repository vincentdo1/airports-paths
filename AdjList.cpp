#include "AdjList.h"
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>
#include <utility>
#include <fstream>
#include <queue>
#include <map>
#include <unordered_map>

/*
    Additional notes:
    -Helper functions could reduce the amount of code needed, specifically a findEdge/Vertex
    that returns an iterator instead of a ptr that could be used for remove
    -We could sort the edges via distance to reduce the amount of time it takes to find
    an edge (via bubblesort), however this probably wouldn't make that much of a difference.
*/

AdjList::AdjList(){
    /*
        No members passed through, default constructor doesn't have to do anything
    */
}
AdjList::AdjList(std::string vertexFile, std::string edgeFile){
    /*
        Parse file and assign edges and vertices.
        Vertices are formatted as: "IO latitude longitude\n"
        Edges are formatted as: "IO source destination\n"
    */
    std::ifstream vertFile (vertexFile, std::ifstream::in);
    std::ifstream edgFile (edgeFile, std::ifstream::in);
    //check for valid files
    if(!vertFile.is_open()){
      std::cerr << "vertexFile invalid" << '\n';
    }
    if(!edgFile.is_open()){
      std::cerr << "edgeFile invalid" << '\n';
    }
    //var decleration for vertFile
    std::string vertID;
    double latitude;
    double longitude;
    //insert every vert into vertList
    while(vertFile >> vertID >> latitude >> longitude){
      insertVertex(vertID, latitude, longitude);
    }
    //var decleration for edge file
    std::string edgeID;
    std::string start;
    std::string end;
    while(edgFile >> edgeID >> start >> end){
      insertEdge(edgeID, findVertex(start), findVertex(end));
    }
    vertFile.close();
    edgFile.close();
    std::list<AdjList::VertexNode*> n;
    std::list<AdjList::VertexNode*> del;
}

//****Rule of 3***
//dtor
AdjList::~AdjList(){
    /*
        Implemented removeVertex function to also removeEdges and delete memory accordingly.
        removeVertex also erases the
    */
    while(edgeList.begin() != edgeList.end())
    {
        auto itr = edgeList.begin();
        removeEdge((*itr)->ID);
    }
    while(vertexList.begin() != vertexList.end())
    {
        auto itr = vertexList.begin();
        removeVertex((*itr)->ID);
    }
}

//and assignment ctor
//TODO: implement
AdjList& AdjList::operator=(const AdjList &other){
    /*
        Use copy constructor to accopmlish operator.
    */
    auto temp = other;
    return *this;
}

//copy ctor
//TODO: implement
AdjList::AdjList(const AdjList& rhs){
    /*
        Delete the currentl AdjList and then reallocate new memory to
        match the new list. Used in assignment constructor?
    */
    AdjList temp = rhs;
}

//Calculate distance between two nodes via Haversine formula (on Wikipedia). This calculation doesn't have
//to be particularly precise.
double AdjList::distance(VertexNode* start, VertexNode* end){
    double radius;
    radius = (6356.752 + 6378.137) / 2.0; //average radius of Earth relative to poles and then equator
    double dLat = ((end->latitude - start->latitude)*M_PI)/180.0;
    double dLon = ((end->longitude - start->longitude)*M_PI)/180.0;
    double startLat = start->latitude*M_PI/180.0;
    double endLat = end->latitude*M_PI/180.0;
    double a = pow(std::sin(dLat/2),2)+pow(std::sin(dLon/2),2)*std::cos(startLat)*std::cos(endLat);
    double c = 2 * asin(sqrt(a));
    return radius*c;
}

std::string AdjList::getID(VertexNode* vertex) {
    return vertex->ID;
}

//Implement a function that changes the distance for testing purposes
void AdjList::changeDistance(std::string name, double value){
    EdgeNode* v = findEdge(name);
    v->distance = value;
}

void AdjList::insertEdge(std::string name, VertexNode* start, VertexNode* end){
    /*
        When we insert an edge, we need to insert the pair into both of
        our vertex's edgelists. We first allocate memory for a new edgeNode,
        then we get its pointer and push its pointer to the list of edges,
        assign its initial values, and push the edge to both the start and
        end vertices' list of edges.

        First check if edge already exists. If so we don't have to allocate new memory.
        We also need to check that both Vertices exist, if not we have to insertVertex.

        Comment out print statements on larger datasets!
    */
    if(start == NULL)
    {
        return;
    }
    if(end == NULL)
    {
        return;
    }
    if(findEdge(name) != NULL)
        return;
    if(findVertex(start->ID) == NULL)
    {
        /*
            The pointer exists but the Vertex does not, indicating something went wrong. The vertex
            may not exist on our list, but the memory may have been allocated? So we have to push
            the vertex onto our list.
        */
        vertexList.push_back(start);
    }
    if(findVertex(end->ID) == NULL)
    {
        vertexList.push_back(end);
    }
    EdgeNode* newEdgePtr = new EdgeNode;
    newEdgePtr->ID = name;
    newEdgePtr->distance = distance(start, end);
    edgeList.push_back(newEdgePtr);
    start->edges.push_back(newEdgePtr);
    end->asEnd.push_back(newEdgePtr);
    std::pair<VertexNode*, VertexNode*> route (start, end);
    newEdgePtr->ends= route;
    //Comment this out on larger datasets
}

void AdjList::insertVertex(std::string name, double lat , double longt){
    /*
        First check if Vertex already exists
    */
    if(findVertex(name) != NULL)
    {
        return;
    }
    /*
        Allocate memory and assign values as necessary. We don't have any
        edges yet so we don't have to worry about it.
    */
    VertexNode* VertexPtr = new VertexNode;
    VertexPtr->ID = name;
    VertexPtr->latitude = lat;
    VertexPtr->longitude = longt;
    vertexList.push_back(VertexPtr);
}

AdjList::VertexNode* AdjList::findVertex(std::string name){
    for(std::list<VertexNode*>::iterator itr = vertexList.begin(); itr != vertexList.end(); itr++){
        if((*itr)->ID == name){
            return *itr;
        }
    }
    return NULL;
}

AdjList::EdgeNode* AdjList::findEdge(std::string name){
    for(auto itr = edgeList.begin(); itr != edgeList.end(); itr++){
        if((*itr)->ID == name){
            return *itr;
        }
    }
    return NULL;
}

std::map<std::string, double> AdjList::returnDistances(){
    std::map<std::string, double> distanceMap;
    for(std::list<EdgeNode*>::iterator itr = edgeList.begin(); itr != edgeList.end(); itr++)
    {
        distanceMap[(*itr)->ID] = (*itr)->distance;
    }
    return distanceMap;
}

//removing vertex/edge make sure to delete nodes from heap
void AdjList::removeVertex(std::string name){
    /*
        When we remove a vertex, we also need to remove any associated
        edges. Thus, we need to parse both the vertexlist and the edgelist.
        We iterate through both lists to check if the vertex exists, or if
        associated edges exist.

        Within the edge list, we need to check both the first and second values
        of the pair, as the edge is not valid if either contains the vertex
        we are trying to delete. Thus, we need to be thorough enough in
        our search.

        Upon finding the vertex within our list, we need to also remove the edge
        from the other vertex on the other side of our edge. Thus, we should look
        to do removeEdge first.

        Thus, we look to perform removeEdge from every single edge within the
        list of edges from the removed vertex.

        We can also call findEdge and findVertex to reduce the amount of code
        that would be needed.
    */
    VertexNode* vptr = findVertex(name);
    if(vptr == NULL)
    {
        return;
    }
    auto size = vptr->edges.size();
    while(size > 0)
    {
        removeEdge(vptr->edges.front()->ID);
        size--;
    }
    size = vptr->asEnd.size();
    while(size > 0)
    {
        removeEdge(vptr->asEnd.front()->ID);
        size--;
    }
    //Now remove the vertex from our overall list of vertices
    for(std::list<VertexNode*>::iterator itr = vertexList.begin(); itr != vertexList.end(); itr++)
    {
        if((*itr)->ID == vptr->ID)
        {
            vertexList.erase(itr);
            delete(vptr);
            return;
        }
    }
}

void AdjList::removeEdge(std::string name){
    /*
        We want to remove our edge between our vertices. We don't have to
        delete anything besides the edgeNode itself, and we just have to
        remove the edge from both its source and destionation's lists of
        edges. Thus we just need to pop/erase the edge from both lists, and
        delete the edge afterwards.
    */

    /*
        First find edge in edgeList, store its pointer. Also store the pointers
        of its corresponding vertices so that we can remove the edge from the lists
        of those vertices.
    */
    EdgeNode* edgePtr = findEdge(name);
    if(edgePtr == NULL)
    {
        return;
    }
    VertexNode* sourcePtr = edgePtr->ends.first;
    VertexNode* destPtr = edgePtr->ends.second;
    /*
        We now have the pointers to our vertices. Now we have to iterate through
        the edge lists of those vertices and remove the edges from their lists.
    */
    for(auto sourceitr = sourcePtr->edges.begin(); sourceitr != sourcePtr->edges.end(); ++sourceitr)
    {
        if((*sourceitr)->ID == edgePtr->ID)
        {
            //Delete from source's edges list
            sourcePtr->edges.erase(sourceitr);
            break;
        }
    }
    //I think the second for loop is suppose to remove the edge in the edge list of the second node of the pair. Switched endPtr to destPtr
    for(std::list<EdgeNode*>::iterator enditr = destPtr->asEnd.begin(); enditr != destPtr->asEnd.end(); enditr++)
    {
        if((*enditr)->ID == edgePtr->ID)
        {
            //Delete from destination's edges list
            destPtr->edges.erase(enditr);
            break;
        }
    }
    /*
        We've removed the edge from its vertices' lists. However, we also need to remove it from
        the overarching list of edges. Parse the list and remove it with the same strategy.

        If we think the list is taking too long to removeEdges, we can at least try to sort the list
        beforehand by distance so that we can find the edge faster. This can also work within the
        edgelists of our vertices.
    */
    for(std::list<EdgeNode*>::iterator itr = edgeList.begin(); itr != edgeList.end(); itr++)
    {
        if((*itr)->ID == edgePtr->ID)
        {
            delete(edgePtr);
            edgeList.erase(itr);
            break;
        }
    }

}

/*
  TODO: param to pcik starting node
*/
void AdjList::BFSTraversal(){
    //Starting off the BFS by initializing the labels for the vertices and edges using map
    std::unordered_map<std::string, bool> visitedVertex;
    std::unordered_map<std::string, bool> visitedEdge;

    //Next part is to start setting labels and traverse
    //for loop might help with disconnected graph
    for(std::list<VertexNode*>::iterator itrVertex = vertexList.begin(); itrVertex != vertexList.end(); itrVertex++){
        //check if node has been visited
        if(visitedVertex.find((*itrVertex)->ID) == visitedVertex.end()){
            std::queue<VertexNode> q;
            visitedVertex[(*itrVertex)->ID] = true;
            q.push(*(*itrVertex));
            while(!q.empty()){
                //v = current node being looked at
                VertexNode v = q.front();
                std::cout << "Node: " << v.ID << '\n';
                q.pop();
                for(std::list<EdgeNode*>::iterator itrEdge = v.edges.begin(); itrEdge != v.edges.end(); itrEdge++){
                    VertexNode w = *((*itrEdge)->ends.first);
                    if(w.ID == v.ID){
                        w = *((*itrEdge)->ends.second);
                    }
                    if(visitedVertex.find(w.ID) == visitedVertex.end()){
                        visitedEdge[(*itrEdge)->ID] = "Discovery";
                        std::cout << "  " << (*itrEdge)->ID << ": Discovery" << '\n';
                        visitedVertex[w.ID] = true;
                        q.push(w);
                    }
                    else {
                        visitedEdge[(*itrEdge)->ID] = "Cross";
                        std::cout << "  " << (*itrEdge)->ID << ": Cross" << '\n';
                    }
                }
            }
        }
    }
}


void AdjList::BFSTraversalID(std::string startVertex){
    //Starting off the BFS by initializing the labels for the vertices and edges using map
    std::unordered_map<std::string, bool> visitedVertex;
    std::unordered_map<std::string, bool> visitedEdge;
    bool first = true;
    //Next part is to start setting labels and traverse
    for(std::list<VertexNode*>::iterator itrVertex = vertexList.begin(); itrVertex != vertexList.end(); itrVertex++){
        //check if node has been visited
        if(visitedVertex.find((*itrVertex)->ID) == visitedVertex.end()){
            std::queue<VertexNode> q;
            if(first){
              visitedVertex[startVertex] = true;
              q.push(*findVertex(startVertex));
              first = false;
            }else{
              visitedVertex[(*itrVertex)->ID] = true;
              q.push(*(*itrVertex));
            }
            while(!q.empty()){
                //v = current node being looked at
                VertexNode v = q.front();
                std::cout << "Node: " << v.ID << '\n';
                q.pop();
                for(std::list<EdgeNode*>::iterator itrEdge = v.edges.begin(); itrEdge != v.edges.end(); itrEdge++){
                    VertexNode w = *((*itrEdge)->ends.first);
                    if(w.ID == v.ID){
                        w = *((*itrEdge)->ends.second);
                    }
                    if(visitedVertex.find(w.ID) == visitedVertex.end()){
                        visitedEdge[(*itrEdge)->ID] = "Discovery";
                        std::cout << "  " << (*itrEdge)->ID << ": Discovery" << '\n';
                        visitedVertex[w.ID] = true;
                        q.push(w);
                    }
                    else {
                        visitedEdge[(*itrEdge)->ID] = "Cross";
                        std::cout << "  " << (*itrEdge)->ID << ": Cross" << '\n';
                    }
                }
            }
        }
    }
}
