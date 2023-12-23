#include "../AdjList.h"
#include "../catch/catch.hpp"
#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <cmath>

TEST_CASE("Testing for path of Floyd Warshall's", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  graph.insertVertex("HZK", 65.952301, -17.426001);
  graph.insertVertex("YFS", 61.76020050048828, -121.23699951171876);
  graph.insertVertex("YMM", 56.653301238999994, -111.22200012200001);
  graph.insertVertex("SBY", 38.34049987792969, -75.51029968261719);

  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  graph.insertEdge("LAEGKA", graph.findVertex("LAE"), graph.findVertex("GKA"));

  graph.insertEdge("GKAHGU", graph.findVertex("GKA"), graph.findVertex("HGU"));

  graph.insertEdge("HGUYMM", graph.findVertex("HGU"), graph.findVertex("YMM"));

  graph.insertEdge("YMMSBY", graph.findVertex("YMM"), graph.findVertex("SBY"));

  graph.insertEdge("SBYHZK", graph.findVertex("SBY"), graph.findVertex("HZK"));

  graph.insertEdge("MAGHZK", graph.findVertex("MAG"), graph.findVertex("HZK"));

  graph.insertEdge("YFSSBY", graph.findVertex("YFS"), graph.findVertex("SBY"));

  graph.insertEdge("HZKYFS", graph.findVertex("HZK"), graph.findVertex("YFS"));

  graph.insertEdge("MAGLAE", graph.findVertex("MAG"), graph.findVertex("LAE"));


  auto adjMatrix = graph.FWAlgorithm();

  REQUIRE(floor(adjMatrix.find("GKA")->second.find("HGU")->second.first) == floor(124.412));
  REQUIRE(floor(adjMatrix.find("GKA")->second.find("GKA")->second.first) == floor(0));
  REQUIRE(floor(adjMatrix.find("GKA")->second.find("HZK")->second.first) == floor(13234));
  REQUIRE(floor(adjMatrix.find("GKA")->second.find("LAE")->second.first) == floor(290));
  REQUIRE(floor(adjMatrix.find("GKA")->second.find("MAG")->second.first) == floor(106.454));
  REQUIRE(floor(adjMatrix.find("GKA")->second.find("SBY")->second.first) == floor(14854));
  REQUIRE(floor(adjMatrix.find("GKA")->second.find("YFS")->second.first) == floor(17753.254));
  REQUIRE(floor(adjMatrix.find("GKA")->second.find("YMM")->second.first) == floor(11548.6));

  REQUIRE(floor(adjMatrix.find("HGU")->second.find("HGU")->second.first) == floor(0));
  REQUIRE(adjMatrix.find("HGU")->second.find("GKA")->second.first == INFINITY);
  REQUIRE(floor(adjMatrix.find("HGU")->second.find("HZK")->second.first) == floor(19469.93));
  REQUIRE((adjMatrix.find("HGU")->second.find("LAE")->second.first) == INFINITY);
  REQUIRE((adjMatrix.find("HGU")->second.find("MAG")->second.first) == INFINITY);
  REQUIRE(floor(adjMatrix.find("HGU")->second.find("SBY")->second.first) == floor(14729.72));
  REQUIRE(std::abs(adjMatrix.find("HGU")->second.find("YFS")->second.first - 23988.9) < 1);
  REQUIRE(std::abs(adjMatrix.find("HGU")->second.find("YMM")->second.first - 11424.6) < 1);

  REQUIRE(adjMatrix.find("HZK")->second.find("HGU")->second.first  == INFINITY);
  REQUIRE(adjMatrix.find("HZK")->second.find("GKA")->second.first == INFINITY);
  REQUIRE(adjMatrix.find("HZK")->second.find("HZK")->second.first == 0);
  REQUIRE(adjMatrix.find("HZK")->second.find("LAE")->second.first == INFINITY);
  REQUIRE(adjMatrix.find("HZK")->second.find("MAG")->second.first == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("HZK")->second.find("SBY")->second.first - 8556.97) < 1);
  REQUIRE(std::abs(adjMatrix.find("HZK")->second.find("YFS")->second.first - 4519) < 1);
  REQUIRE(adjMatrix.find("HZK")->second.find("YMM")->second.first == INFINITY);

  REQUIRE(std::abs(adjMatrix.find("LAE")->second.find("HGU")->second.first  - 281.414) < 1);
  REQUIRE(std::abs(adjMatrix.find("LAE")->second.find("GKA")->second.first - 157.014) < 1);
  REQUIRE(std::abs(adjMatrix.find("LAE")->second.find("HZK")->second.first - 13391.268) < 1);
  REQUIRE(adjMatrix.find("LAE")->second.find("LAE")->second.first == 0);
  REQUIRE(std::abs(adjMatrix.find("LAE")->second.find("MAG")->second.first -263.468) <1);
  REQUIRE(std::abs(adjMatrix.find("LAE")->second.find("SBY")->second.first - 15011.014) < 1);
  REQUIRE(std::abs(adjMatrix.find("LAE")->second.find("YFS")->second.first - 17910.9) < 1);
  REQUIRE(std::abs(adjMatrix.find("LAE")->second.find("YMM")->second.first - 11706.014) < 1);

  //double check rest
  REQUIRE(std::abs(adjMatrix.find("MAG")->second.find("GKA")->second.first  - 340.484) < 1);
  REQUIRE(std::abs(adjMatrix.find("MAG")->second.find("HGU")->second.first - 464.884) < 1);
  REQUIRE(std::abs(adjMatrix.find("MAG")->second.find("HZK")->second.first - 13127.6) < 1);
  REQUIRE(std::abs(adjMatrix.find("MAG")->second.find("LAE")->second.first - 183.47) < 1);
  REQUIRE(adjMatrix.find("MAG")->second.find("MAG")->second.first == 0);
  REQUIRE(std::abs(adjMatrix.find("MAG")->second.find("SBY")->second.first - 15194.64) < 1);
  REQUIRE(std::abs(adjMatrix.find("MAG")->second.find("YFS")->second.first - 17646.6) < 1);
  REQUIRE(std::abs(adjMatrix.find("MAG")->second.find("YMM")->second.first - 11889.484) < 1);

  REQUIRE(std::abs(adjMatrix.find("SBY")->second.find("GKA")->second.first) == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("SBY")->second.find("HGU")->second.first) == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("SBY")->second.find("HZK")->second.first - 4740.21) < 1);
  REQUIRE(std::abs(adjMatrix.find("SBY")->second.find("LAE")->second.first) == INFINITY);
  REQUIRE(adjMatrix.find("SBY")->second.find("MAG")->second.first == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("SBY")->second.find("SBY")->second.first) == 0);
  REQUIRE(std::abs(adjMatrix.find("SBY")->second.find("YFS")->second.first - 9259.21) < 1);
  REQUIRE(std::abs(adjMatrix.find("SBY")->second.find("YMM")->second.first) == INFINITY);

  REQUIRE(std::abs(adjMatrix.find("YFS")->second.find("GKA")->second.first) == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("YFS")->second.find("HGU")->second.first) == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("YFS")->second.find("HZK")->second.first - 8778.18) < 1);
  REQUIRE(std::abs(adjMatrix.find("YFS")->second.find("LAE")->second.first) == INFINITY);
  REQUIRE(adjMatrix.find("YFS")->second.find("MAG")->second.first == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("YFS")->second.find("SBY")->second.first - 4037.97) < 1);
  REQUIRE(std::abs(adjMatrix.find("YFS")->second.find("YFS")->second.first) == 0);
  REQUIRE(std::abs(adjMatrix.find("YFS")->second.find("YMM")->second.first) == INFINITY);

  REQUIRE(std::abs(adjMatrix.find("YMM")->second.find("GKA")->second.first) == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("YMM")->second.find("HGU")->second.first) == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("YMM")->second.find("HZK")->second.first - 8045.33) < 1);
  REQUIRE(std::abs(adjMatrix.find("YMM")->second.find("LAE")->second.first) == INFINITY);
  REQUIRE(adjMatrix.find("YMM")->second.find("MAG")->second.first == INFINITY);
  REQUIRE(std::abs(adjMatrix.find("YMM")->second.find("SBY")->second.first - 3305.12) < 1);
  REQUIRE(std::abs(adjMatrix.find("YMM")->second.find("YFS")->second.first - 12564.33) < 1);
  REQUIRE(std::abs(adjMatrix.find("YMM")->second.find("YMM")->second.first) == 0);
}

TEST_CASE("Test FW #2: line", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 3);
  graph.insertVertex("D", 3, 3);
  graph.insertVertex("E", 4, 4);

  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.insertEdge("CB", graph.findVertex("C"), graph.findVertex("B"));
  graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
  graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));
  graph.insertEdge("DE", graph.findVertex("D"), graph.findVertex("E"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));

  graph.changeDistance("AB", 1);
  graph.changeDistance("BA", 1);
  graph.changeDistance("BC", 1);
  graph.changeDistance("CB", 1);
  graph.changeDistance("CD", 1);
  graph.changeDistance("DC", 1);
  graph.changeDistance("DE", 1);
  graph.changeDistance("ED", 1);

  REQUIRE(graph.FWSingleOutput("A", "E") == std::pair<std::string, double>("ABCDE", 4));
  REQUIRE(graph.FWSingleOutput("C", "A") == std::pair<std::string, double>("CBA", 2));
  REQUIRE(graph.FWSingleOutput("E", "B") == std::pair<std::string, double>("EDCB", 3));
  REQUIRE(graph.FWSingleOutput("A", "D") == std::pair<std::string, double>("ABCD", 3));
}



TEST_CASE("Test FW #3: complex", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);
  graph.insertVertex("D", 3, 3);
  graph.insertVertex("E", 4, 4);

  graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
  graph.insertEdge("CA", graph.findVertex("C"), graph.findVertex("A"));
  graph.insertEdge("EC", graph.findVertex("E"), graph.findVertex("C"));
  graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
  graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));
  graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
  graph.insertEdge("EB", graph.findVertex("E"), graph.findVertex("B"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
  graph.insertEdge("DE", graph.findVertex("D"), graph.findVertex("E"));

  graph.changeDistance("AC", 1);
  graph.changeDistance("CA", 1);
  graph.changeDistance("EC", 3);
  graph.changeDistance("CE", 3);
  graph.changeDistance("DC", 5);
  graph.changeDistance("CD", 5);
  graph.changeDistance("AB", 3);
  graph.changeDistance("BA", 3);
  graph.changeDistance("BE", 5);
  graph.changeDistance("EB", 5);
  graph.changeDistance("ED", 7);
  graph.changeDistance("DE", 7);

    double outDist;
    std::map<std::string, std::string> paths;
    auto FWMap = graph.FWAlgorithm();
    std::vector<std::string> path;
    AdjList::VertexNode* cur = graph.findVertex("A");
    auto target = graph.findVertex("D");
    outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    bool noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"A", "C", "D"});

    REQUIRE(graph.FWSingleOutput("A", "D") == std::pair<std::string, double>("ACD", outDist));


    path.clear();
    cur = graph.findVertex("A");
    target = graph.findVertex("E");
    outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"A", "C", "E"});

    REQUIRE(graph.FWSingleOutput("A", "E") == std::pair<std::string, double>("ACE", outDist));

    path.clear();
    cur = graph.findVertex("B");
    target = graph.findVertex("C");
    outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"B", "A", "C"});

    REQUIRE(graph.FWSingleOutput("B", "C") == std::pair<std::string, double>("BAC", outDist));


    path.clear();
    cur = graph.findVertex("B");
    target = graph.findVertex("D");
    outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"B", "A", "C", "D"});

    REQUIRE(graph.FWSingleOutput("B", "D") == std::pair<std::string, double>("BACD", outDist));


    path.clear();
    cur = graph.findVertex("C");
    target = graph.findVertex("B");
    outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"C", "A", "B"});

    REQUIRE(graph.FWSingleOutput("C", "B") == std::pair<std::string, double>("CAB", outDist));


    path.clear();
    cur = graph.findVertex("D");
    target = graph.findVertex("A");
    outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"D", "C", "A"});

    REQUIRE(graph.FWSingleOutput("D", "A") == std::pair<std::string, double>("DCA", outDist));


    path.clear();
    cur = graph.findVertex("D");
    target = graph.findVertex("B");
    outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"D", "C", "A", "B"});

    REQUIRE(graph.FWSingleOutput("D", "B") == std::pair<std::string, double>("DCAB", outDist));


    path.clear();
    cur = graph.findVertex("E");
    target = graph.findVertex("A");
    outDist = FWMap.find(cur->ID)->second.find(target->ID)->second.first;
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"E", "C", "A"});

    REQUIRE(graph.FWSingleOutput("E", "A") == std::pair<std::string, double>("ECA", outDist));

    REQUIRE(graph.FWSingleOutput("A", "C") == std::pair<std::string, double>("AC", 1));

    graph.insertVertex("F", 1, 0);
    REQUIRE(graph.FWSingleOutput("A", "F") == std::pair<std::string, double>("NULL", 0));
}

TEST_CASE("Test FW #4: complex", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("A", 1, 1);
  graph.insertVertex("B", 2, 2);
  graph.insertVertex("C", 3, 3);
  graph.insertVertex("D", 4, 4);
  graph.insertVertex("E", 5, 5);

  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("AD", graph.findVertex("A"), graph.findVertex("D"));
  graph.insertEdge("DA", graph.findVertex("D"), graph.findVertex("A"));
  graph.insertEdge("EB", graph.findVertex("E"), graph.findVertex("B"));
  graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
  graph.insertEdge("DE", graph.findVertex("D"), graph.findVertex("E"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));

  graph.changeDistance("AB", 5);
  graph.changeDistance("BA", 5);
  graph.changeDistance("AD", 6);
  graph.changeDistance("DA", 6);
  graph.changeDistance("EB", 6);
  graph.changeDistance("BE", 6);
  graph.changeDistance("ED", 3);
  graph.changeDistance("DE", 3);
  graph.changeDistance("BC", 1);
  graph.changeDistance("CD", 1);

  std::map<std::string, std::string> paths;
  auto FWMap = graph.FWAlgorithm();
  std::vector<std::string> path;
  AdjList::VertexNode* cur = graph.findVertex("A");
  auto target = graph.findVertex("E");
  bool noPath = false;
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
  REQUIRE(path == std::vector<std::string>{"A", "D", "E"});

  path.clear();
    cur = graph.findVertex("B");
    target = graph.findVertex("D");
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"B", "C", "D"});

    path.clear();
    cur = graph.findVertex("C");
    target = graph.findVertex("A");
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"C", "D", "A"});

    path.clear();
    cur = graph.findVertex("C");
    target = graph.findVertex("E");
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"C", "D", "E"});

    path.clear();
    cur = graph.findVertex("D");
    target = graph.findVertex("B");
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"D", "E", "B"});

    path.clear();
    cur = graph.findVertex("E");
    target = graph.findVertex("A");
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"E", "D", "A"});

    path.clear();
    cur = graph.findVertex("E");
    target = graph.findVertex("C");
    noPath = false;
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
    REQUIRE(path == std::vector<std::string>{"E", "B", "C"});

}

TEST_CASE("Test BC #1: simple", "[weight=1]") {
    AdjList graph;

    //insert 5 vertices, make sure coordinates have appropriate distances
    graph.insertVertex("A", 2, 0);
    graph.insertVertex("B", 2, 2);
    graph.insertVertex("C", 1, 1);
    graph.insertVertex("D", 0, 0);
    graph.insertVertex("E", 0, 2);

    //unnconnected vertex
    graph.insertVertex("U", 3, 1);
    //insert 8 edges, connecting all vertices to C and only C bi directionally
    graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
    graph.insertEdge("CA", graph.findVertex("C"), graph.findVertex("A"));
    graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
    graph.insertEdge("CB", graph.findVertex("C"), graph.findVertex("B"));
    graph.insertEdge("EC", graph.findVertex("E"), graph.findVertex("C"));
    graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
    graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));

    graph.changeDistance("AC", 1);
    graph.changeDistance("CA", 1);
    graph.changeDistance("BC", 1);
    graph.changeDistance("CB", 1);
    graph.changeDistance("EC", 1);
    graph.changeDistance("CE", 1);
    graph.changeDistance("DC", 1);
    graph.changeDistance("CD", 1);

    auto BCMap = graph.BCAlgorithm();
    REQUIRE(BCMap["A"] == 0);
    REQUIRE(BCMap["B"] == 0);
    REQUIRE(BCMap["D"] == 0);
    REQUIRE(BCMap["E"] == 0);
    REQUIRE(BCMap["F"] == 0);
    REQUIRE(BCMap["C"] == 12.0/20.0);

}

TEST_CASE("Test BC #2: simple", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 3);
  graph.insertVertex("D", 3, 3);
  graph.insertVertex("E", 4, 4);

  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.insertEdge("CB", graph.findVertex("C"), graph.findVertex("B"));
  graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
  graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));
  graph.insertEdge("DE", graph.findVertex("D"), graph.findVertex("E"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));

  graph.changeDistance("AB", 1);
  graph.changeDistance("BA", 1);
  graph.changeDistance("BC", 1);
  graph.changeDistance("CB", 1);
  graph.changeDistance("CD", 1);
  graph.changeDistance("DC", 1);
  graph.changeDistance("DE", 1);
  graph.changeDistance("ED", 1);

  auto BCMap = graph.BCAlgorithm();
  REQUIRE(BCMap["A"] == 0);
  REQUIRE(BCMap["B"] == (6.0/12.0));
  REQUIRE(BCMap["C"] == (8.0/12.0));
  REQUIRE(BCMap["D"] == (6.0/12.0));
  REQUIRE(BCMap["E"] == 0);
}

TEST_CASE("Test BC #3: medium", "[weight=1]") {
    AdjList graph;

    //insert 5 vertices, C is significantly farther
    graph.insertVertex("A", 2, 0);
    graph.insertVertex("B", 2, 2);
    graph.insertVertex("C", 6, 6);
    graph.insertVertex("D", 0, 0);
    graph.insertVertex("E", 0, 2);

    //insert edges, C is significantly farther from all other vertices
    graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
    graph.insertEdge("CA", graph.findVertex("C"), graph.findVertex("A"));
    graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
    graph.insertEdge("CB", graph.findVertex("C"), graph.findVertex("B"));
    graph.insertEdge("EC", graph.findVertex("E"), graph.findVertex("C"));
    graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
    graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
    graph.insertEdge("AD", graph.findVertex("A"), graph.findVertex("D"));
    graph.insertEdge("DA", graph.findVertex("D"), graph.findVertex("A"));
    graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
    graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
    graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
    graph.insertEdge("EB", graph.findVertex("E"), graph.findVertex("B"));
    graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
    graph.insertEdge("DE", graph.findVertex("D"), graph.findVertex("E"));

    graph.changeDistance("AC", 5);
    graph.changeDistance("CA", 5);
    graph.changeDistance("BC", 5);
    graph.changeDistance("CB", 5);
    graph.changeDistance("EC", 5);
    graph.changeDistance("CE", 5);
    graph.changeDistance("DC", 5);
    graph.changeDistance("CD", 5);
    graph.changeDistance("AD", 1);
    graph.changeDistance("DA", 1);
    graph.changeDistance("AB", 1);
    graph.changeDistance("BA", 1);
    graph.changeDistance("BE", 1);
    graph.changeDistance("EB", 1);
    graph.changeDistance("ED", 2);
    graph.changeDistance("DE", 2);


    std::map<std::string, double> BCMap = graph.BCAlgorithm();
    REQUIRE(BCMap["A"] == (2.0/12.0));
    REQUIRE(BCMap["B"] == (2.0/12.0));
    REQUIRE(BCMap["C"] == 0);
    REQUIRE(BCMap["D"] == 0);
    REQUIRE(BCMap["E"] == 0);


}

TEST_CASE("Test BC #4: complex" , "[weight=1]") {
    AdjList graph;

    graph.insertVertex("A", 0, 0);
    graph.insertVertex("B", 1, 1);
    graph.insertVertex("C", 2, 2);
    graph.insertVertex("D", 3, 3);
    graph.insertVertex("E", 4, 4);

    graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
    graph.insertEdge("CA", graph.findVertex("C"), graph.findVertex("A"));
    graph.insertEdge("EC", graph.findVertex("E"), graph.findVertex("C"));
    graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
    graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
    graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
    graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
    graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
    graph.insertEdge("EB", graph.findVertex("E"), graph.findVertex("B"));
    graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
    graph.insertEdge("DE", graph.findVertex("D"), graph.findVertex("E"));

    graph.changeDistance("AC", 1);
    graph.changeDistance("CA", 1);
    graph.changeDistance("EC", 3);
    graph.changeDistance("CE", 3);
    graph.changeDistance("DC", 5);
    graph.changeDistance("CD", 5);
    graph.changeDistance("AB", 3);
    graph.changeDistance("BA", 3);
    graph.changeDistance("BE", 5);
    graph.changeDistance("EB", 5);
    graph.changeDistance("ED", 7);
    graph.changeDistance("DE", 7);

    std::map<std::string, double> BCMap = graph.BCAlgorithm();
    REQUIRE(BCMap["A"] == (1.0/3.0));
    REQUIRE(BCMap["B"] == 0);
    REQUIRE(BCMap["C"] == (1.0/2.0));
    REQUIRE(BCMap["D"] == 0);
    REQUIRE(BCMap["E"] == 0);

}

TEST_CASE("BC Test #5: complex", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("A", 1, 1);
  graph.insertVertex("B", 2, 2);
  graph.insertVertex("C", 3, 3);
  graph.insertVertex("D", 4, 4);
  graph.insertVertex("E", 5, 5);

  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("AD", graph.findVertex("A"), graph.findVertex("D"));
  graph.insertEdge("DA", graph.findVertex("D"), graph.findVertex("A"));
  graph.insertEdge("EB", graph.findVertex("E"), graph.findVertex("B"));
  graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
  graph.insertEdge("DE", graph.findVertex("D"), graph.findVertex("E"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));

  graph.changeDistance("AB", 5);
  graph.changeDistance("BA", 5);
  graph.changeDistance("AD", 6);
  graph.changeDistance("DA", 6);
  graph.changeDistance("EB", 6);
  graph.changeDistance("BE", 6);
  graph.changeDistance("ED", 3);
  graph.changeDistance("DE", 3);
  graph.changeDistance("BC", 1);
  graph.changeDistance("CD", 1);

  std::map<std::string, double> BCMap = graph.BCAlgorithm();
  REQUIRE(BCMap["A"] == 0);
  REQUIRE(BCMap["B"] == (3.0/12.0));
  REQUIRE(BCMap["C"] == (2.0/12.0));
  REQUIRE(BCMap["D"] == (1.0/2.0));
  REQUIRE(BCMap["E"] == (3.0/12.0));
}

TEST_CASE("FW Directional Test #1", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);
  graph.insertVertex("D", 3, 3);
  graph.insertVertex("E", 4, 4);

  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
  graph.insertEdge("CA", graph.findVertex("C"), graph.findVertex("A"));
  graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
  graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));
  
  graph.changeDistance("AB", 2);
  graph.changeDistance("BC", 8);
  graph.changeDistance("BE", 4);
  graph.changeDistance("CA", 7);
  graph.changeDistance("CE", 3);
  graph.changeDistance("ED", 1);
  graph.changeDistance("DC", 1);

  REQUIRE(graph.FWSingleOutput("A", "B") == std::pair<std::string, double>("AB", 2.0));
  REQUIRE(graph.FWSingleOutput("A", "C") == std::pair<std::string, double>("ABEDC", 8.0));
  REQUIRE(graph.FWSingleOutput("A", "D") == std::pair<std::string, double>("ABED", 7.0));
  REQUIRE(graph.FWSingleOutput("A", "E") == std::pair<std::string, double>("ABE", 6.0));
  REQUIRE(graph.FWSingleOutput("B", "A") == std::pair<std::string, double>("BEDCA", 13.0));
  REQUIRE(graph.FWSingleOutput("B", "C") == std::pair<std::string, double>("BEDC", 6.0));
  REQUIRE(graph.FWSingleOutput("B", "D") == std::pair<std::string, double>("BED", 5.0));
  REQUIRE(graph.FWSingleOutput("B", "E") == std::pair<std::string, double>("BE", 4.0));
  REQUIRE(graph.FWSingleOutput("C", "A") == std::pair<std::string, double>("CA", 7.0));
  REQUIRE(graph.FWSingleOutput("C", "B") == std::pair<std::string, double>("CAB", 9.0));
  REQUIRE(graph.FWSingleOutput("C", "D") == std::pair<std::string, double>("CED", 4.0));
  REQUIRE(graph.FWSingleOutput("C", "E") == std::pair<std::string, double>("CE", 3.0));
  REQUIRE(graph.FWSingleOutput("D", "A") == std::pair<std::string, double>("DCA", 8.0));
  REQUIRE(graph.FWSingleOutput("D", "B") == std::pair<std::string, double>("DCAB", 10.0));
  REQUIRE(graph.FWSingleOutput("D", "C") == std::pair<std::string, double>("DC", 1.0));
  REQUIRE(graph.FWSingleOutput("D", "E") == std::pair<std::string, double>("DCE", 4.0));
  REQUIRE(graph.FWSingleOutput("E", "A") == std::pair<std::string, double>("EDCA", 9.0));
  REQUIRE(graph.FWSingleOutput("E", "B") == std::pair<std::string, double>("EDCAB", 11.0));
  REQUIRE(graph.FWSingleOutput("E", "C") == std::pair<std::string, double>("EDC", 2.0));
  REQUIRE(graph.FWSingleOutput("E", "D") == std::pair<std::string, double>("ED", 1.0));

}

TEST_CASE("BC Directional Test #1", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);
  graph.insertVertex("D", 3, 3);
  graph.insertVertex("E", 4, 4);

  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
  graph.insertEdge("CA", graph.findVertex("C"), graph.findVertex("A"));
  graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
  graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));
  
  graph.changeDistance("AB", 2);
  graph.changeDistance("BC", 8);
  graph.changeDistance("BE", 4);
  graph.changeDistance("CA", 7);
  graph.changeDistance("CE", 3);
  graph.changeDistance("ED", 1);
  graph.changeDistance("DC", 1);

  auto BCMap = graph.BCAlgorithm();
  REQUIRE(BCMap["A"] == (3.0/12.0));
  REQUIRE(BCMap["B"] == (3.0/12.0));
  REQUIRE(BCMap["C"] == (6.0/12.0));
  REQUIRE(BCMap["D"] == (6.0/12.0));
  REQUIRE(BCMap["E"] == (6.0/12.0));
}

TEST_CASE("FW and BC Disconnected Graphs #1: Simple", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("A", 2, 0);
  graph.insertVertex("B", 2, 2);
  graph.insertVertex("C", 1, 1);
  graph.insertVertex("D", 0, 0);

  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
  graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));

  graph.changeDistance("AB", 1);
  graph.changeDistance("BA", 1);
  graph.changeDistance("CD", 1);
  graph.changeDistance("DC", 1);

  REQUIRE(graph.FWSingleOutput("A", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "B") == std::pair<std::string, double>("AB", 1));
  REQUIRE(graph.FWSingleOutput("B", "A") == std::pair<std::string, double>("BA", 1));
  REQUIRE(graph.FWSingleOutput("C", "D") == std::pair<std::string, double>("CD", 1));
  REQUIRE(graph.FWSingleOutput("D", "C") == std::pair<std::string, double>("DC", 1));
  
  std::map<std::string, double> BCMap = graph.BCAlgorithm();
  REQUIRE(BCMap["A"] == 0.0);
  REQUIRE(BCMap["B"] == 0.0);
  REQUIRE(BCMap["C"] == 0.0);
  REQUIRE(BCMap["D"] == 0.0);
}

TEST_CASE("FW and BC Disconnected Graphs #2: Simple Directional", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("A", 2, 0);
  graph.insertVertex("B", 2, 2);
  graph.insertVertex("C", 1, 1);
  graph.insertVertex("D", 0, 0);

  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("DC", graph.findVertex("D"), graph.findVertex("C"));

  graph.changeDistance("BA", 1);
  graph.changeDistance("DC", 1);

  REQUIRE(graph.FWSingleOutput("A", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "A") == std::pair<std::string, double>("BA", 1));
  REQUIRE(graph.FWSingleOutput("C", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "C") == std::pair<std::string, double>("DC", 1));
  
  std::map<std::string, double> BCMap = graph.BCAlgorithm();
  REQUIRE(BCMap["A"] == 0.0);
  REQUIRE(BCMap["B"] == 0.0);
  REQUIRE(BCMap["C"] == 0.0);
  REQUIRE(BCMap["D"] == 0.0);
}

TEST_CASE("FW and BC Disconnected Graphs #3: Complex", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("A", 2, 0);
  graph.insertVertex("B", 2, 2);
  graph.insertVertex("C", 1, 1);
  graph.insertVertex("D", 0, 0);
  graph.insertVertex("E", 4, 5);
  graph.insertVertex("F", 6, 7);

  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.insertEdge("CB", graph.findVertex("C"), graph.findVertex("B"));
  graph.insertEdge("DE", graph.findVertex("D"), graph.findVertex("E"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
  graph.insertEdge("EF", graph.findVertex("E"), graph.findVertex("F"));
  graph.insertEdge("FE", graph.findVertex("F"), graph.findVertex("E"));


  graph.changeDistance("AB", 1);
  graph.changeDistance("BA", 1);
  graph.changeDistance("BC", 1);
  graph.changeDistance("CB", 1);
  graph.changeDistance("DE", 1);
  graph.changeDistance("ED", 1);
  graph.changeDistance("EF", 1);
  graph.changeDistance("FE", 1);

  REQUIRE(graph.FWSingleOutput("A", "B") == std::pair<std::string, double>("AB", 1));
  REQUIRE(graph.FWSingleOutput("A", "C") == std::pair<std::string, double>("ABC", 2));
  REQUIRE(graph.FWSingleOutput("B", "A") == std::pair<std::string, double>("BA", 1));
  REQUIRE(graph.FWSingleOutput("B", "C") == std::pair<std::string, double>("BC", 1));
  REQUIRE(graph.FWSingleOutput("C", "A") == std::pair<std::string, double>("CBA", 2));
  REQUIRE(graph.FWSingleOutput("C", "B") == std::pair<std::string, double>("CB", 1));
  REQUIRE(graph.FWSingleOutput("D", "E") == std::pair<std::string, double>("DE", 1));
  REQUIRE(graph.FWSingleOutput("D", "F") == std::pair<std::string, double>("DEF", 2));
  REQUIRE(graph.FWSingleOutput("E", "D") == std::pair<std::string, double>("ED", 1));
  REQUIRE(graph.FWSingleOutput("E", "F") == std::pair<std::string, double>("EF", 1));
  REQUIRE(graph.FWSingleOutput("F", "D") == std::pair<std::string, double>("FED", 2));
  REQUIRE(graph.FWSingleOutput("F", "E") == std::pair<std::string, double>("FE", 1));

  REQUIRE(graph.FWSingleOutput("A", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "E").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "F").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "E").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "F").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "E").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "F").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("E", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("E", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("E", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("F", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("F", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("F", "C").first == "NULL");

  std::map<std::string, double> BCMap = graph.BCAlgorithm();
  REQUIRE(BCMap["A"] == 0.0);
  REQUIRE(BCMap["B"] == (2.0/20.0));
  REQUIRE(BCMap["C"] == 0.0);
  REQUIRE(BCMap["D"] == 0.0);
  REQUIRE(BCMap["E"] == (2.0/20.0));
  REQUIRE(BCMap["F"] == 0.0);
}

TEST_CASE("FW and BC Disconnected Graphs #3: Complex Directional", "[weight=1]") {
AdjList graph;

  graph.insertVertex("A", 2, 0);
  graph.insertVertex("B", 2, 2);
  graph.insertVertex("C", 1, 1);
  graph.insertVertex("D", 0, 0);
  graph.insertVertex("E", 4, 5);
  graph.insertVertex("F", 6, 7);

  graph.insertEdge("BA", graph.findVertex("B"), graph.findVertex("A"));
  graph.insertEdge("CB", graph.findVertex("C"), graph.findVertex("B"));
  graph.insertEdge("ED", graph.findVertex("E"), graph.findVertex("D"));
  graph.insertEdge("FE", graph.findVertex("F"), graph.findVertex("E"));


  graph.changeDistance("BA", 1);
  graph.changeDistance("CB", 1);
  graph.changeDistance("ED", 1);
  graph.changeDistance("FE", 1);

  REQUIRE(graph.FWSingleOutput("A", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "E").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "F").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "E").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "F").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "D").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "E").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "F").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("E", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("E", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("E", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("F", "A").first == "NULL");
  REQUIRE(graph.FWSingleOutput("F", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("F", "C").first == "NULL");

  REQUIRE(graph.FWSingleOutput("A", "B").first == "NULL");
  REQUIRE(graph.FWSingleOutput("A", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("B", "A") == std::pair<std::string, double>("BA", 1));
  REQUIRE(graph.FWSingleOutput("B", "C").first == "NULL");
  REQUIRE(graph.FWSingleOutput("C", "A") == std::pair<std::string, double>("CBA", 2));
  REQUIRE(graph.FWSingleOutput("C", "B") == std::pair<std::string, double>("CB", 1));
  REQUIRE(graph.FWSingleOutput("D", "E").first == "NULL");
  REQUIRE(graph.FWSingleOutput("D", "F").first == "NULL");
  REQUIRE(graph.FWSingleOutput("E", "D") == std::pair<std::string, double>("ED", 1));
  REQUIRE(graph.FWSingleOutput("E", "F").first == "NULL");
  REQUIRE(graph.FWSingleOutput("F", "D") == std::pair<std::string, double>("FED", 2));
  REQUIRE(graph.FWSingleOutput("F", "E") == std::pair<std::string, double>("FE", 1));

  std::map<std::string, double> BCMap = graph.BCAlgorithm();
  REQUIRE(BCMap["A"] == 0.0);
  REQUIRE(BCMap["B"] == (1.0/20.0));
  REQUIRE(BCMap["C"] == 0.0);
  REQUIRE(BCMap["D"] == 0.0);
  REQUIRE(BCMap["E"] == (1.0/20.0));
  REQUIRE(BCMap["F"] == 0.0);
}

