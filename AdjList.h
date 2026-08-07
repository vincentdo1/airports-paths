#pragma once

#include <list>
#include <iostream>
#include <string>
#include <utility>
#include <cmath>
#include <queue>
#include <map>
#include <vector>

class AdjList
{
  private:
    //internal structure of AdjList
    //avoid compiler error


    //main make up of AdjList


  public:
  struct VertexNode;

    struct EdgeNode {
      std::string ID;
      double distance;
      std::pair<VertexNode*, VertexNode*> ends;
    };
    struct VertexNode {
      std::string ID;
      double latitude;
      double longitude;
      std::list<EdgeNode*> edges;
      std::list<EdgeNode*> asEnd;
    };
    std::list<VertexNode*> vertexList;
    std::list<EdgeNode*> edgeList;
    //ctors
    AdjList();
    AdjList(std::string vertexFile, std::string edgeFile);

    //****Rule of 3***
    //dtor
    ~AdjList();

    //This graph owns raw pointers and is only ever loaded once and then read, so
    //copying it doesn't make sense. Deleting these also retires the old copy ctor
    //and assignment, which were broken (the copy ctor recursed into itself and the
    //assignment left the target unchanged).
    AdjList& operator =(const AdjList &other) = delete;
    AdjList(const AdjList& rhs) = delete;
    //*******************

    //Calculate distance between two nodes
    double distance(VertexNode* start, VertexNode* end);

    std::string getID(VertexNode* vertex);

    void changeDistance(std::string name, double value);

    void insertEdge(std::string name, VertexNode* start, VertexNode* end);

    void insertVertex(std::string name, double lat , double longt);

    //finding vertex/edge
    VertexNode* findVertex(std::string name);
    EdgeNode* findEdge(std::string name);

    std::map<std::string, double> returnDistances();

    //removing vertex/edge make sure to delete nodes from heap
    void removeVertex(std::string name);
    void removeEdge(std::string name);

    //BFS traversal
    void BFSTraversal();
    void BFSTraversalID(std::string startVertex);

    //Floyd-Warshall's Algorithm
    //first is distrance, second is path
    std::map<std::string, std::map<std::string, std::pair<double, VertexNode*>>> FWAlgorithm();

    //Between-ness centrality
    std::map<std::string, double> BCAlgorithm();

    std::pair<std::string, double> FWSingleOutput(std::string start, std::string end);

    double getShortestDistance(std::string start, std::string end);

    //BFS shortest path (fewest hops) between two airports
    std::vector<std::string> BFSPath(std::string start, std::string end);

    //Dijkstra's shortest path by distance between two airports
    //first is the path in order, second is the total distance
    std::pair<std::vector<std::string>, double> DijkstraPath(std::string start, std::string end);
};
