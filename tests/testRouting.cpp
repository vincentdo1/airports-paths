#include "../AdjList.h"
#include "../catch/catch.hpp"
#include <string>
#include <vector>
#include <utility>

/*
    These tests cover the two routing algorithms used by the API: BFSPath, which
    returns the route with the fewest hops, and DijkstraPath, which returns the
    route with the shortest total distance. The little graphs are built by hand the
    same way the FW and BC tests are, so the expected routes are easy to follow.
    Distances are set with changeDistance so we control the edge weights directly.
*/

TEST_CASE("BFSPath finds the fewest-hop route", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);
  graph.insertVertex("D", 3, 3);

  //Two ways from A to D, both two hops. A-B-D is inserted first, so BFS finds it first.
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BD", graph.findVertex("B"), graph.findVertex("D"));
  graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
  graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));

  REQUIRE(graph.BFSPath("A", "D") == std::vector<std::string>{"A", "B", "D"});
  REQUIRE(graph.BFSPath("A", "B") == std::vector<std::string>{"A", "B"});
}

TEST_CASE("BFSPath counts hops and ignores distance", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);

  //A direct A->C hop plus a longer-way-around A->B->C detour.
  graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));

  //Even if the detour were shorter in kilometers, BFS still takes the single hop.
  REQUIRE(graph.BFSPath("A", "C") == std::vector<std::string>{"A", "C"});
}

TEST_CASE("BFSPath handles same airport, unknown airport, and no route", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));

  //Same source and destination is a zero-hop route
  REQUIRE(graph.BFSPath("A", "A") == std::vector<std::string>{"A"});
  //Unknown airports can't be routed
  REQUIRE(graph.BFSPath("A", "Z").empty());
  REQUIRE(graph.BFSPath("Z", "A").empty());
  //Edges are directed, so there is no way back from B to A
  REQUIRE(graph.BFSPath("B", "A").empty());
}

TEST_CASE("DijkstraPath finds the shortest route by distance", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);

  //One direct but long hop versus a cheaper two-hop path.
  graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.changeDistance("AC", 10);
  graph.changeDistance("AB", 1);
  graph.changeDistance("BC", 1);

  std::pair<std::vector<std::string>, double> route = graph.DijkstraPath("A", "C");
  REQUIRE(route.first == std::vector<std::string>{"A", "B", "C"});
  REQUIRE(route.second == 2.0);

  //By hops, though, the direct edge wins. The two modes disagree on purpose here.
  REQUIRE(graph.BFSPath("A", "C") == std::vector<std::string>{"A", "C"});
}

TEST_CASE("DijkstraPath prefers the cheaper of several routes", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);
  graph.insertVertex("D", 3, 3);

  //A->B->D costs 2+2=4, A->C->D costs 1+1=2.
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BD", graph.findVertex("B"), graph.findVertex("D"));
  graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
  graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
  graph.changeDistance("AB", 2);
  graph.changeDistance("BD", 2);
  graph.changeDistance("AC", 1);
  graph.changeDistance("CD", 1);

  std::pair<std::vector<std::string>, double> route = graph.DijkstraPath("A", "D");
  REQUIRE(route.first == std::vector<std::string>{"A", "C", "D"});
  REQUIRE(route.second == 2.0);
}

TEST_CASE("DijkstraPath handles same airport, unknown airport, and no route", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.changeDistance("AB", 5);

  std::pair<std::vector<std::string>, double> same = graph.DijkstraPath("A", "A");
  REQUIRE(same.first == std::vector<std::string>{"A"});
  REQUIRE(same.second == 0.0);

  //Unknown airport and a directed dead-end both give an empty path
  REQUIRE(graph.DijkstraPath("A", "Z").first.empty());
  REQUIRE(graph.DijkstraPath("B", "A").first.empty());

  std::pair<std::vector<std::string>, double> ab = graph.DijkstraPath("A", "B");
  REQUIRE(ab.first == std::vector<std::string>{"A", "B"});
  REQUIRE(ab.second == 5.0);
}
