#include "../AdjList.h"
#include "../catch/catch.hpp"
#include <string>
#include <vector>
#include <utility>

TEST_CASE("BFSPath finds the fewest-hop route", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);
  graph.insertVertex("D", 3, 3);

  // Both routes have two hops; insertion order breaks the tie.
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

  graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));

  REQUIRE(graph.BFSPath("A", "C") == std::vector<std::string>{"A", "C"});
}

TEST_CASE("BFSPath handles same airport, unknown airport, and no route", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));

  REQUIRE(graph.BFSPath("A", "A") == std::vector<std::string>{"A"});
  REQUIRE(graph.BFSPath("A", "Z").empty());
  REQUIRE(graph.BFSPath("Z", "A").empty());
  REQUIRE(graph.BFSPath("B", "A").empty());
}

TEST_CASE("DijkstraPath finds the shortest route by distance", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);

  graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
  graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
  graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
  graph.changeDistance("AC", 10);
  graph.changeDistance("AB", 1);
  graph.changeDistance("BC", 1);

  std::pair<std::vector<std::string>, double> route = graph.DijkstraPath("A", "C");
  REQUIRE(route.first == std::vector<std::string>{"A", "B", "C"});
  REQUIRE(route.second == 2.0);

  // BFS still chooses the direct edge.
  REQUIRE(graph.BFSPath("A", "C") == std::vector<std::string>{"A", "C"});
}

TEST_CASE("DijkstraPath prefers the cheaper of several routes", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("A", 0, 0);
  graph.insertVertex("B", 1, 1);
  graph.insertVertex("C", 2, 2);
  graph.insertVertex("D", 3, 3);

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

  REQUIRE(graph.DijkstraPath("A", "Z").first.empty());
  REQUIRE(graph.DijkstraPath("B", "A").first.empty());

  std::pair<std::vector<std::string>, double> ab = graph.DijkstraPath("A", "B");
  REQUIRE(ab.first == std::vector<std::string>{"A", "B"});
  REQUIRE(ab.second == 5.0);
}
