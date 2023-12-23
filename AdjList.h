#pragma once

#include <list>
#include <iostream>
#include <string>
#include <utility>
#include <cmath>
#include <queue>
#include <map>

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

    //and assignment ctor
    AdjList& operator =(const AdjList &other);

    //copy ctor
    AdjList(const AdjList& rhs);
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
};
