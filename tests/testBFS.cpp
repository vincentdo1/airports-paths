#include "../AdjList.h"
#include "../catch/catch.hpp"
#include <iostream>

// BFS - simple traversal, single directional
TEST_CASE("BFS Simple #1: 4 vertices, edges in a line, default call", "[weight=1]") {
    AdjList graph;
    
    graph.insertVertex("A", 0, 0);
    graph.insertVertex("B", 0, 1);
    graph.insertVertex("C", 1, 1);
    graph.insertVertex("D", 0, 1);

    graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
    graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);

    graph.BFSTraversal();
}

//BFS - simple traversal, single directional, custom call 
TEST_CASE("BFS Simple #2: 4 vertices, edges in a line, custom call (start at C)", "[weight=1]") {
    AdjList graph;
    
    graph.insertVertex("A", 0, 0);
    graph.insertVertex("B", 0, 1);
    graph.insertVertex("C", 1, 1);
    graph.insertVertex("D", 0, 1);

    graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
    graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);

    graph.BFSTraversalID("C");

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);

}

//BFS - simple traversal, single directional, custom call 
TEST_CASE("BFS Simple #3: 4 vertices, edges in a line, custom call (start at C)", "[weight=1]") {
    AdjList graph;
    
    graph.insertVertex("A", 0, 0);
    graph.insertVertex("B", 0, 1);
    graph.insertVertex("C", 1, 1);
    graph.insertVertex("D", 0, 1);

    graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
    graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);

    graph.BFSTraversalID("C");

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);

}

TEST_CASE("BFS Complex #1: Lecture example, default call", "[weight=1]") {
    AdjList graph;

    graph.insertVertex("A", 0, 0);
    graph.insertVertex("B", 1, 1);
    graph.insertVertex("C", 2, 2);
    graph.insertVertex("D", 3, 3);
    graph.insertVertex("E", 4, 4);
    graph.insertVertex("F", 5, 5);
    graph.insertVertex("G", 6, 6);
    graph.insertVertex("H", 7, 7);

    graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
    graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
    graph.insertEdge("AD", graph.findVertex("A"), graph.findVertex("D"));
    graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
    graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
    graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
    graph.insertEdge("CF", graph.findVertex("C"), graph.findVertex("F"));
    graph.insertEdge("DF", graph.findVertex("D"), graph.findVertex("F"));
    graph.insertEdge("DH", graph.findVertex("D"), graph.findVertex("H"));
    graph.insertEdge("EG", graph.findVertex("E"), graph.findVertex("G"));
    graph.insertEdge("FG", graph.findVertex("F"), graph.findVertex("G"));
    graph.insertEdge("GH", graph.findVertex("G"), graph.findVertex("H"));

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );
    REQUIRE( graph.findVertex("E") != nullptr );
    REQUIRE( graph.findVertex("F") != nullptr );
    REQUIRE( graph.findVertex("G") != nullptr );
    REQUIRE( graph.findVertex("H") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("AC") != nullptr);
    REQUIRE( graph.findEdge("AD") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);
    REQUIRE( graph.findEdge("BE") != nullptr);
    REQUIRE( graph.findEdge("CE") != nullptr);
    REQUIRE( graph.findEdge("CF") != nullptr);
    REQUIRE( graph.findEdge("DF") != nullptr);
    REQUIRE( graph.findEdge("DH") != nullptr);
    REQUIRE( graph.findEdge("EG") != nullptr);
    REQUIRE( graph.findEdge("FG") != nullptr);
    REQUIRE( graph.findEdge("GH") != nullptr);

    graph.BFSTraversal();

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );
    REQUIRE( graph.findVertex("E") != nullptr );
    REQUIRE( graph.findVertex("F") != nullptr );
    REQUIRE( graph.findVertex("G") != nullptr );
    REQUIRE( graph.findVertex("H") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("AC") != nullptr);
    REQUIRE( graph.findEdge("AD") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);
    REQUIRE( graph.findEdge("BE") != nullptr);
    REQUIRE( graph.findEdge("CE") != nullptr);
    REQUIRE( graph.findEdge("CF") != nullptr);
    REQUIRE( graph.findEdge("DF") != nullptr);
    REQUIRE( graph.findEdge("DH") != nullptr);
    REQUIRE( graph.findEdge("EG") != nullptr);
    REQUIRE( graph.findEdge("FG") != nullptr);
    REQUIRE( graph.findEdge("GH") != nullptr);

}

TEST_CASE("BFS Complex #2: Lecture example, custom call start at A", "[weight=1]") {
    AdjList graph;

    graph.insertVertex("A", 0, 0);
    graph.insertVertex("B", 1, 1);
    graph.insertVertex("C", 2, 2);
    graph.insertVertex("D", 3, 3);
    graph.insertVertex("E", 4, 4);
    graph.insertVertex("F", 5, 5);
    graph.insertVertex("G", 6, 6);
    graph.insertVertex("H", 7, 7);

    graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
    graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
    graph.insertEdge("AD", graph.findVertex("A"), graph.findVertex("D"));
    graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
    graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
    graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
    graph.insertEdge("CF", graph.findVertex("C"), graph.findVertex("F"));
    graph.insertEdge("DF", graph.findVertex("D"), graph.findVertex("F"));
    graph.insertEdge("DH", graph.findVertex("D"), graph.findVertex("H"));
    graph.insertEdge("EG", graph.findVertex("E"), graph.findVertex("G"));
    graph.insertEdge("FG", graph.findVertex("F"), graph.findVertex("G"));
    graph.insertEdge("GH", graph.findVertex("G"), graph.findVertex("H"));

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );
    REQUIRE( graph.findVertex("E") != nullptr );
    REQUIRE( graph.findVertex("F") != nullptr );
    REQUIRE( graph.findVertex("G") != nullptr );
    REQUIRE( graph.findVertex("H") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("AC") != nullptr);
    REQUIRE( graph.findEdge("AD") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);
    REQUIRE( graph.findEdge("BE") != nullptr);
    REQUIRE( graph.findEdge("CE") != nullptr);
    REQUIRE( graph.findEdge("CF") != nullptr);
    REQUIRE( graph.findEdge("DF") != nullptr);
    REQUIRE( graph.findEdge("DH") != nullptr);
    REQUIRE( graph.findEdge("EG") != nullptr);
    REQUIRE( graph.findEdge("FG") != nullptr);
    REQUIRE( graph.findEdge("GH") != nullptr);

    graph.BFSTraversalID("A");

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );
    REQUIRE( graph.findVertex("E") != nullptr );
    REQUIRE( graph.findVertex("F") != nullptr );
    REQUIRE( graph.findVertex("G") != nullptr );
    REQUIRE( graph.findVertex("H") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("AC") != nullptr);
    REQUIRE( graph.findEdge("AD") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);
    REQUIRE( graph.findEdge("BE") != nullptr);
    REQUIRE( graph.findEdge("CE") != nullptr);
    REQUIRE( graph.findEdge("CF") != nullptr);
    REQUIRE( graph.findEdge("DF") != nullptr);
    REQUIRE( graph.findEdge("DH") != nullptr);
    REQUIRE( graph.findEdge("EG") != nullptr);
    REQUIRE( graph.findEdge("FG") != nullptr);
    REQUIRE( graph.findEdge("GH") != nullptr);

}

TEST_CASE("BFS Complex #3: Lecture example, custom call start at C", "[weight=1]") {
    AdjList graph;

    graph.insertVertex("A", 0, 0);
    graph.insertVertex("B", 1, 1);
    graph.insertVertex("C", 2, 2);
    graph.insertVertex("D", 3, 3);
    graph.insertVertex("E", 4, 4);
    graph.insertVertex("F", 5, 5);
    graph.insertVertex("G", 6, 6);
    graph.insertVertex("H", 7, 7);

    graph.insertEdge("AB", graph.findVertex("A"), graph.findVertex("B"));
    graph.insertEdge("AC", graph.findVertex("A"), graph.findVertex("C"));
    graph.insertEdge("AD", graph.findVertex("A"), graph.findVertex("D"));
    graph.insertEdge("BC", graph.findVertex("B"), graph.findVertex("C"));
    graph.insertEdge("CD", graph.findVertex("C"), graph.findVertex("D"));
    graph.insertEdge("BE", graph.findVertex("B"), graph.findVertex("E"));
    graph.insertEdge("CE", graph.findVertex("C"), graph.findVertex("E"));
    graph.insertEdge("CF", graph.findVertex("C"), graph.findVertex("F"));
    graph.insertEdge("DF", graph.findVertex("D"), graph.findVertex("F"));
    graph.insertEdge("DH", graph.findVertex("D"), graph.findVertex("H"));
    graph.insertEdge("EG", graph.findVertex("E"), graph.findVertex("G"));
    graph.insertEdge("FG", graph.findVertex("F"), graph.findVertex("G"));
    graph.insertEdge("GH", graph.findVertex("G"), graph.findVertex("H"));

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );
    REQUIRE( graph.findVertex("E") != nullptr );
    REQUIRE( graph.findVertex("F") != nullptr );
    REQUIRE( graph.findVertex("G") != nullptr );
    REQUIRE( graph.findVertex("H") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("AC") != nullptr);
    REQUIRE( graph.findEdge("AD") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);
    REQUIRE( graph.findEdge("BE") != nullptr);
    REQUIRE( graph.findEdge("CE") != nullptr);
    REQUIRE( graph.findEdge("CF") != nullptr);
    REQUIRE( graph.findEdge("DF") != nullptr);
    REQUIRE( graph.findEdge("DH") != nullptr);
    REQUIRE( graph.findEdge("EG") != nullptr);
    REQUIRE( graph.findEdge("FG") != nullptr);
    REQUIRE( graph.findEdge("GH") != nullptr);

    graph.BFSTraversalID("C");

    REQUIRE( graph.findVertex("A") != nullptr );
    REQUIRE( graph.findVertex("B") != nullptr );
    REQUIRE( graph.findVertex("C") != nullptr );
    REQUIRE( graph.findVertex("D") != nullptr );
    REQUIRE( graph.findVertex("E") != nullptr );
    REQUIRE( graph.findVertex("F") != nullptr );
    REQUIRE( graph.findVertex("G") != nullptr );
    REQUIRE( graph.findVertex("H") != nullptr );

    REQUIRE( graph.findEdge("AB") != nullptr);
    REQUIRE( graph.findEdge("AC") != nullptr);
    REQUIRE( graph.findEdge("AD") != nullptr);
    REQUIRE( graph.findEdge("BC") != nullptr);
    REQUIRE( graph.findEdge("CD") != nullptr);
    REQUIRE( graph.findEdge("BE") != nullptr);
    REQUIRE( graph.findEdge("CE") != nullptr);
    REQUIRE( graph.findEdge("CF") != nullptr);
    REQUIRE( graph.findEdge("DF") != nullptr);
    REQUIRE( graph.findEdge("DH") != nullptr);
    REQUIRE( graph.findEdge("EG") != nullptr);
    REQUIRE( graph.findEdge("FG") != nullptr);
    REQUIRE( graph.findEdge("GH") != nullptr);

}