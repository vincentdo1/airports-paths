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

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);

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

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);

    graph.BFSTraversalID("C");

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);

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

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);

    graph.BFSTraversalID("C");

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);

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

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );
    REQUIRE( graph.findVertex("E") != NULL );
    REQUIRE( graph.findVertex("F") != NULL );
    REQUIRE( graph.findVertex("G") != NULL );
    REQUIRE( graph.findVertex("H") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("AC") != NULL);
    REQUIRE( graph.findEdge("AD") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);
    REQUIRE( graph.findEdge("BE") != NULL);
    REQUIRE( graph.findEdge("CE") != NULL);
    REQUIRE( graph.findEdge("CF") != NULL);
    REQUIRE( graph.findEdge("DF") != NULL);
    REQUIRE( graph.findEdge("DH") != NULL);
    REQUIRE( graph.findEdge("EG") != NULL);
    REQUIRE( graph.findEdge("FG") != NULL);
    REQUIRE( graph.findEdge("GH") != NULL);

    graph.BFSTraversal();

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );
    REQUIRE( graph.findVertex("E") != NULL );
    REQUIRE( graph.findVertex("F") != NULL );
    REQUIRE( graph.findVertex("G") != NULL );
    REQUIRE( graph.findVertex("H") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("AC") != NULL);
    REQUIRE( graph.findEdge("AD") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);
    REQUIRE( graph.findEdge("BE") != NULL);
    REQUIRE( graph.findEdge("CE") != NULL);
    REQUIRE( graph.findEdge("CF") != NULL);
    REQUIRE( graph.findEdge("DF") != NULL);
    REQUIRE( graph.findEdge("DH") != NULL);
    REQUIRE( graph.findEdge("EG") != NULL);
    REQUIRE( graph.findEdge("FG") != NULL);
    REQUIRE( graph.findEdge("GH") != NULL);

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

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );
    REQUIRE( graph.findVertex("E") != NULL );
    REQUIRE( graph.findVertex("F") != NULL );
    REQUIRE( graph.findVertex("G") != NULL );
    REQUIRE( graph.findVertex("H") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("AC") != NULL);
    REQUIRE( graph.findEdge("AD") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);
    REQUIRE( graph.findEdge("BE") != NULL);
    REQUIRE( graph.findEdge("CE") != NULL);
    REQUIRE( graph.findEdge("CF") != NULL);
    REQUIRE( graph.findEdge("DF") != NULL);
    REQUIRE( graph.findEdge("DH") != NULL);
    REQUIRE( graph.findEdge("EG") != NULL);
    REQUIRE( graph.findEdge("FG") != NULL);
    REQUIRE( graph.findEdge("GH") != NULL);

    graph.BFSTraversalID("A");

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );
    REQUIRE( graph.findVertex("E") != NULL );
    REQUIRE( graph.findVertex("F") != NULL );
    REQUIRE( graph.findVertex("G") != NULL );
    REQUIRE( graph.findVertex("H") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("AC") != NULL);
    REQUIRE( graph.findEdge("AD") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);
    REQUIRE( graph.findEdge("BE") != NULL);
    REQUIRE( graph.findEdge("CE") != NULL);
    REQUIRE( graph.findEdge("CF") != NULL);
    REQUIRE( graph.findEdge("DF") != NULL);
    REQUIRE( graph.findEdge("DH") != NULL);
    REQUIRE( graph.findEdge("EG") != NULL);
    REQUIRE( graph.findEdge("FG") != NULL);
    REQUIRE( graph.findEdge("GH") != NULL);

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

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );
    REQUIRE( graph.findVertex("E") != NULL );
    REQUIRE( graph.findVertex("F") != NULL );
    REQUIRE( graph.findVertex("G") != NULL );
    REQUIRE( graph.findVertex("H") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("AC") != NULL);
    REQUIRE( graph.findEdge("AD") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);
    REQUIRE( graph.findEdge("BE") != NULL);
    REQUIRE( graph.findEdge("CE") != NULL);
    REQUIRE( graph.findEdge("CF") != NULL);
    REQUIRE( graph.findEdge("DF") != NULL);
    REQUIRE( graph.findEdge("DH") != NULL);
    REQUIRE( graph.findEdge("EG") != NULL);
    REQUIRE( graph.findEdge("FG") != NULL);
    REQUIRE( graph.findEdge("GH") != NULL);

    graph.BFSTraversalID("C");

    REQUIRE( graph.findVertex("A") != NULL );
    REQUIRE( graph.findVertex("B") != NULL );
    REQUIRE( graph.findVertex("C") != NULL );
    REQUIRE( graph.findVertex("D") != NULL );
    REQUIRE( graph.findVertex("E") != NULL );
    REQUIRE( graph.findVertex("F") != NULL );
    REQUIRE( graph.findVertex("G") != NULL );
    REQUIRE( graph.findVertex("H") != NULL );

    REQUIRE( graph.findEdge("AB") != NULL);
    REQUIRE( graph.findEdge("AC") != NULL);
    REQUIRE( graph.findEdge("AD") != NULL);
    REQUIRE( graph.findEdge("BC") != NULL);
    REQUIRE( graph.findEdge("CD") != NULL);
    REQUIRE( graph.findEdge("BE") != NULL);
    REQUIRE( graph.findEdge("CE") != NULL);
    REQUIRE( graph.findEdge("CF") != NULL);
    REQUIRE( graph.findEdge("DF") != NULL);
    REQUIRE( graph.findEdge("DH") != NULL);
    REQUIRE( graph.findEdge("EG") != NULL);
    REQUIRE( graph.findEdge("FG") != NULL);
    REQUIRE( graph.findEdge("GH") != NULL);

}