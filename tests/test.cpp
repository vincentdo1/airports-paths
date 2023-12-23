#include "../AdjList.h"
#include "../catch/catch.hpp"
#include <iostream>


// Adjlist default constructor
TEST_CASE("AdjList's custom constructor works", "[weight=1]") {
  AdjList graph("data/nodes.txt", "data/edges.txt");
  //all vertices should exist... testing for some
  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );
  REQUIRE( graph.findVertex("HGU") != NULL  );
  REQUIRE( graph.findVertex("LAE") != NULL  );
  REQUIRE( graph.findVertex("YMM") != NULL  );

  //all edges should exist... testing for some
  REQUIRE( graph.findEdge("GKAMAG") != NULL  );
  REQUIRE( graph.findEdge("MAGGKA") != NULL  );
  REQUIRE( graph.findEdge("HGUGKA") != NULL  );
  REQUIRE( graph.findEdge("MAGHGU") != NULL  );
  REQUIRE( graph.findEdge("LAEMAG") != NULL  );
  REQUIRE( graph.findEdge("LAEHGU") != NULL  );
}

TEST_CASE("AdjList's distance() function works #1", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  //calculating distance
  double d = graph.distance(graph.findVertex("GKA"), graph.findVertex("MAG"));
  //actual distance
  double compare = 106.71;
  double diff = d - compare;
  if (diff < 0) {
    diff = diff * -1;
  }
  //if in range
  REQUIRE( diff < 1 );
}
TEST_CASE("AdjList's distance() function works #2", "[weight=1]") {
  AdjList graph;

  graph.insertVertex("JFK",40.63980103,-73.77890015);
  graph.insertVertex("PEK",40.080101013183594,116.58499908447266);

  //calculating distance
  double d = graph.distance(graph.findVertex("JFK"), graph.findVertex("PEK"));
  //actual distance
  double compare = 10971;
  double diff = d - compare;
  if (diff < 0) {
    diff = diff * -1;
  }
  //if in range
  REQUIRE( diff < 2 );
}

TEST_CASE("AdjList's findVertex()  test", "[weight=1]") {
  AdjList graph;

  //vertex should not exist!
  REQUIRE( graph.findVertex("GKA") == NULL  );
  REQUIRE( graph.findVertex("MAG") == NULL  );

  //testing weird inputs
  REQUIRE( graph.findVertex("UUU") == NULL  );
  REQUIRE( graph.findVertex("   ") == NULL  );
  REQUIRE( graph.findVertex("") == NULL  );
  REQUIRE( graph.findVertex("!@#") == NULL  );
  REQUIRE( graph.findVertex("!@#$%^&*()") == NULL  );
  REQUIRE( graph.findVertex("ADA") == NULL  );

}

TEST_CASE("AdjList's findEdge()  test", "[weight=1]") {
  AdjList graph;

  //Edge should not exist!
  REQUIRE( graph.findEdge("GKAMAG") == NULL  );

  //testing weird inputs
  REQUIRE( graph.findEdge("      ") == NULL  );
  REQUIRE( graph.findEdge("") == NULL  );
  REQUIRE( graph.findEdge("NNNNNN") == NULL  );
  REQUIRE( graph.findEdge("QWERTY") == NULL  );
  REQUIRE( graph.findEdge("QWERTYUIOP") == NULL  );
  REQUIRE( graph.findEdge("!@#$%^&*()") == NULL  );
  REQUIRE( graph.findEdge("ADADAD") == NULL  );
}


TEST_CASE("AdjList's insertEdge() simple test", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  //Edge should not exist!
  REQUIRE( graph.findEdge("GKAMAG") == NULL  );

  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  //edge should exist
  REQUIRE( graph.findEdge("GKAMAG") != NULL  );
}

TEST_CASE("AdjList's insertEdge() multiple test", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));
  // attempting to add duplicate
  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  // add edge in other direction
  graph.insertEdge("MAGGKA", graph.findVertex("MAG"), graph.findVertex("GKA"));

  //edge should exist
  REQUIRE( graph.findEdge("GKAMAG") != NULL  );
  REQUIRE( graph.findEdge("MAGGKA") != NULL  );
}

TEST_CASE("AdjList's insertEdge() complex test", "[weight=1]") {
  AdjList graph;

  //inserting 4 nodes
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  //inserting edges
  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  graph.insertEdge("MAGGKA", graph.findVertex("MAG"), graph.findVertex("GKA"));
  graph.insertEdge("HGUGKA", graph.findVertex("HGU"), graph.findVertex("GKA"));
  graph.insertEdge("MAGHGU", graph.findVertex("MAG"), graph.findVertex("HGU"));
  graph.insertEdge("LAEMAG", graph.findVertex("LAE"), graph.findVertex("MAG"));
  graph.insertEdge("LAEHGU", graph.findVertex("LAE"), graph.findVertex("HGU"));


  //check if edge exists
  REQUIRE( graph.findEdge("GKAMAG") != NULL  );
  REQUIRE( graph.findEdge("MAGGKA") != NULL  );
  REQUIRE( graph.findEdge("HGUGKA") != NULL  );
  REQUIRE( graph.findEdge("MAGHGU") != NULL  );
  REQUIRE( graph.findEdge("LAEMAG") != NULL  );
  REQUIRE( graph.findEdge("LAEHGU") != NULL  );

}

TEST_CASE("AdjList's insertVertex() simple test", "[weight=1]") {
  AdjList graph;
  //vertex should not exist!
  REQUIRE( graph.findVertex("GKA") == NULL  );
  REQUIRE( graph.findVertex("MAG") == NULL  );

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  //testing for incorrect latitudes and longs; remove line if unapplicable!!!
  graph.insertVertex("GKA",33,33);

  //vertex should exist!
  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );
}

TEST_CASE("AdjList's insertVertex() multiple test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );
  REQUIRE( graph.findVertex("HGU") != NULL  );
  REQUIRE( graph.findVertex("LAE") != NULL  );
}

TEST_CASE("AdjList's insertVertex() complex test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  graph.insertVertex("HZK", 65.952301, -17.426001);
  graph.insertVertex("YFS", 61.76020050048828, -121.23699951171876);
  graph.insertVertex("YMM", 56.653301238999994, -111.22200012200001);
  graph.insertVertex("SBY", 38.34049987792969, -75.51029968261719);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );
  REQUIRE( graph.findVertex("HGU") != NULL  );
  REQUIRE( graph.findVertex("LAE") != NULL  );
  REQUIRE( graph.findVertex("HZK") != NULL  );
  REQUIRE( graph.findVertex("YFS") != NULL  );
  REQUIRE( graph.findVertex("YMM") != NULL  );
  REQUIRE( graph.findVertex("SBY") != NULL  );
}

TEST_CASE("AdjList's removeEdge() simple test", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  //Edge should not exist!
  REQUIRE( graph.findEdge("GKAMAG") == NULL  );

  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  //edge should exist
  REQUIRE( graph.findEdge("GKAMAG") != NULL  );

  //remove edge
  graph.removeEdge("GKAMAG");

  //edge should not exist
  REQUIRE( graph.findEdge("GKAMAG") == NULL  );
}
TEST_CASE("AdjList's removeEdge() multiple test", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));
  // attempting to add duplicate
  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  // add edge in other direction
  graph.insertEdge("MAGGKA", graph.findVertex("MAG"), graph.findVertex("GKA"));

  graph.removeEdge("GKAMAG");
  graph.removeEdge("GKAMAG");
  graph.removeEdge("MAGGKA");
  graph.removeEdge("MAGGKA");

  //edge should not exist
  REQUIRE( graph.findEdge("GKAMAG") == NULL  );
  REQUIRE( graph.findEdge("MAGGKA") == NULL  );
}

TEST_CASE("AdjList's removeEdge() complex test", "[weight=1]") {
  AdjList graph;

  //inserting 4 nodes
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  //inserting edges
  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  graph.insertEdge("MAGGKA", graph.findVertex("MAG"), graph.findVertex("GKA"));
  graph.insertEdge("HGUGKA", graph.findVertex("HGU"), graph.findVertex("GKA"));
  graph.insertEdge("MAGHGU", graph.findVertex("MAG"), graph.findVertex("HGU"));
  graph.insertEdge("LAEMAG", graph.findVertex("LAE"), graph.findVertex("MAG"));
  graph.insertEdge("LAEHGU", graph.findVertex("LAE"), graph.findVertex("HGU"));

  //removing edges
  graph.removeEdge("GKAMAG");
  graph.removeEdge("GKAMAG");
  graph.removeEdge("MAGGKA");
  graph.removeEdge("MAGGKA");
  graph.removeEdge("LAEHGU");
  graph.removeEdge("MAGHGU");
  graph.removeEdge("LAEMAG");
  graph.removeEdge("HGUGKA");
  graph.removeEdge("MAGGKA");

  //edge should not exist
  REQUIRE( graph.findEdge("GKAMAG") == NULL  );
  REQUIRE( graph.findEdge("MAGGKA") == NULL  );
  REQUIRE( graph.findEdge("LAEHGU") == NULL  );
  REQUIRE( graph.findEdge("MAGHGU") == NULL  );
  REQUIRE( graph.findEdge("LAEMAG") == NULL  );
  REQUIRE( graph.findEdge("HGUGKA") == NULL  );
}

TEST_CASE("AdjList's removeVertex() simple test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  graph.insertEdge("MAGGKA", graph.findVertex("MAG"), graph.findVertex("GKA"));

  //vertex should exist!
  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );

  graph.removeVertex("GKA");

  //vertex should be removed!
  REQUIRE( graph.findVertex("GKA") == NULL  );

  graph.removeVertex("MAG");

  REQUIRE( graph.findVertex("MAG") == NULL  );

  //edge should also be removed
  REQUIRE( graph.findEdge("MAGGKA") == NULL  );
}
TEST_CASE("AdjList's removeVertex() multiple test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  graph.insertEdge("HGULAE", graph.findVertex("HGU"), graph.findVertex("LAE"));
  graph.insertEdge("LAEHGU", graph.findVertex("LAE"), graph.findVertex("HGU"));
  graph.insertEdge("GKAHGU", graph.findVertex("GKA"), graph.findVertex("HGU"));

  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );
  REQUIRE( graph.findVertex("HGU") != NULL  );
  REQUIRE( graph.findVertex("LAE") != NULL  );

  //removing vertices
  graph.removeVertex("GKA");
  graph.removeVertex("LAE");
  graph.removeVertex("HGU");
  graph.removeVertex("MAG");


  REQUIRE( graph.findVertex("GKA") == NULL  );
  REQUIRE( graph.findVertex("MAG") == NULL  );
  REQUIRE( graph.findVertex("HGU") == NULL  );
  REQUIRE( graph.findVertex("LAE") == NULL  );

  //edges should also be removed
  REQUIRE( graph.findEdge("HGULAE") == NULL  );
  REQUIRE( graph.findEdge("LAEHGU") == NULL  );
  REQUIRE( graph.findEdge("GKAHGU") == NULL  );
}
TEST_CASE("AdjList's removeVertex() complex test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  graph.insertVertex("HZK", 65.952301, -17.426001);
  graph.insertVertex("YFS", 61.76020050048828, -121.23699951171876);
  graph.insertVertex("YMM", 56.653301238999994, -111.22200012200001);
  graph.insertVertex("SBY", 38.34049987792969, -75.51029968261719);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  //Look at this over!!!!

  graph.insertEdge("HGULAE", graph.findVertex("HGU"), graph.findVertex("LAE"));
  graph.insertEdge("LAEHGU", graph.findVertex("LAE"), graph.findVertex("HGU"));
  graph.insertEdge("GKAHGU", graph.findVertex("GKA"), graph.findVertex("HGU"));

  graph.insertEdge("YFSSBY", graph.findVertex("YFS"), graph.findVertex("SBY"));
  graph.insertEdge("SBYYMM", graph.findVertex("SBY"), graph.findVertex("YMM"));
  //failing at this action
  graph.insertEdge("YMMHGU", graph.findVertex("YMM"), graph.findVertex("HGU"));

  graph.insertEdge("HGUYMM", graph.findVertex("HGU"), graph.findVertex("YMM"));


  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );
  REQUIRE( graph.findVertex("HGU") != NULL  );
  REQUIRE( graph.findVertex("LAE") != NULL  );
  REQUIRE( graph.findVertex("HZK") != NULL  );
  REQUIRE( graph.findVertex("YFS") != NULL  );
  REQUIRE( graph.findVertex("YMM") != NULL  );
  REQUIRE( graph.findVertex("SBY") != NULL  );

  //removing vertices
  graph.removeVertex("GKA");
  graph.removeVertex("MAG");
  graph.removeVertex("HGU");
  graph.removeVertex("LAE");
  graph.removeVertex("HZK");
  graph.removeVertex("YFS");
  //action produces error
  graph.removeVertex("YMM");

  graph.removeVertex("SBY");


  //testing for removal
  REQUIRE( graph.findVertex("GKA") == NULL  );
  REQUIRE( graph.findVertex("MAG") == NULL  );
  REQUIRE( graph.findVertex("HGU") == NULL  );
  REQUIRE( graph.findVertex("LAE") == NULL  );
  REQUIRE( graph.findVertex("HZK") == NULL  );
  REQUIRE( graph.findVertex("YFS") == NULL  );
  REQUIRE( graph.findVertex("YMM") == NULL  );
  REQUIRE( graph.findVertex("SBY") == NULL  );

  //edges should also be removed
  REQUIRE( graph.findEdge("HGULAE") == NULL  );
  REQUIRE( graph.findEdge("LAEHGU") == NULL  );
  REQUIRE( graph.findEdge("GKAHGU") == NULL  );

  //These assertions produce weird stuff
  REQUIRE( graph.findEdge("YFSSBY") == NULL  );
  REQUIRE( graph.findEdge("SBYYMM") == NULL  );
  REQUIRE( graph.findEdge("YMMHGU") == NULL  );
  REQUIRE( graph.findEdge("HGUYMM") == NULL  );
}

TEST_CASE("AdjList's removeVertex() weird test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  graph.insertVertex("HZK", 65.952301, -17.426001);
  graph.insertVertex("YFS", 61.76020050048828, -121.23699951171876);
  graph.insertVertex("YMM", 56.653301238999994, -111.22200012200001);
  graph.insertVertex("SBY", 38.34049987792969, -75.51029968261719);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  //Look at this over!!!!

  graph.insertEdge("HGULAE", graph.findVertex("HGU"), graph.findVertex("LAE"));
  graph.insertEdge("LAEHGU", graph.findVertex("LAE"), graph.findVertex("HGU"));
  graph.insertEdge("GKAHGU", graph.findVertex("GKA"), graph.findVertex("HGU"));

  graph.insertEdge("YFSSBY", graph.findVertex("YFS"), graph.findVertex("SBY"));
  graph.insertEdge("SBYYMM", graph.findVertex("SBY"), graph.findVertex("YMM"));
  //failing at this action
  graph.insertEdge("YMMHGU", graph.findVertex("YMM"), graph.findVertex("HGU"));

  graph.insertEdge("HGUYMM", graph.findVertex("HGU"), graph.findVertex("YMM"));


  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );
  REQUIRE( graph.findVertex("HGU") != NULL  );
  REQUIRE( graph.findVertex("LAE") != NULL  );
  REQUIRE( graph.findVertex("HZK") != NULL  );
  REQUIRE( graph.findVertex("YFS") != NULL  );
  REQUIRE( graph.findVertex("YMM") != NULL  );
  REQUIRE( graph.findVertex("SBY") != NULL  );

  //removing vertices
  graph.removeVertex("GKA");
  //removing GKA again
  graph.removeVertex("GKA");
  graph.removeVertex("GKA");

  graph.removeVertex("MAG");
  graph.removeVertex("LAE");
  graph.removeVertex("HZK");
  graph.removeVertex("YFS");
  graph.removeVertex("YMM");

  graph.removeVertex("SBY");


  //testing for removal
  REQUIRE( graph.findVertex("GKA") == NULL  );
  REQUIRE( graph.findVertex("MAG") == NULL  );
  REQUIRE( graph.findVertex("LAE") == NULL  );
  REQUIRE( graph.findVertex("HZK") == NULL  );
  REQUIRE( graph.findVertex("YFS") == NULL  );
  REQUIRE( graph.findVertex("YMM") == NULL  );
  REQUIRE( graph.findVertex("SBY") == NULL  );

  //edges should also be removed
  REQUIRE( graph.findEdge("YFSSBY") == NULL  );
  REQUIRE( graph.findEdge("SBYYMM") == NULL  );
}

TEST_CASE("AdjList's removeVertex() random order removal test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  graph.insertVertex("HZK", 65.952301, -17.426001);
  graph.insertVertex("YFS", 61.76020050048828, -121.23699951171876);
  graph.insertVertex("YMM", 56.653301238999994, -111.22200012200001);
  graph.insertVertex("SBY", 38.34049987792969, -75.51029968261719);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  graph.insertEdge("HGULAE", graph.findVertex("HGU"), graph.findVertex("LAE"));
  graph.insertEdge("LAEHGU", graph.findVertex("LAE"), graph.findVertex("HGU"));
  graph.insertEdge("GKAHGU", graph.findVertex("GKA"), graph.findVertex("HGU"));

  graph.insertEdge("YFSSBY", graph.findVertex("YFS"), graph.findVertex("SBY"));
  graph.insertEdge("SBYYMM", graph.findVertex("SBY"), graph.findVertex("YMM"));
  graph.insertEdge("YMMHGU", graph.findVertex("YMM"), graph.findVertex("HGU"));

  graph.insertEdge("HGUYMM", graph.findVertex("HGU"), graph.findVertex("YMM"));


  REQUIRE( graph.findVertex("GKA") != NULL  );
  REQUIRE( graph.findVertex("MAG") != NULL  );
  REQUIRE( graph.findVertex("HGU") != NULL  );
  REQUIRE( graph.findVertex("LAE") != NULL  );
  REQUIRE( graph.findVertex("HZK") != NULL  );
  REQUIRE( graph.findVertex("YFS") != NULL  );
  REQUIRE( graph.findVertex("YMM") != NULL  );
  REQUIRE( graph.findVertex("SBY") != NULL  );

  //removing vertices
  graph.removeVertex("LAE");
  graph.removeVertex("MAG");
  graph.removeVertex("HGU");
  graph.removeVertex("SBY");
  graph.removeVertex("YMM");
  graph.removeVertex("HZK");
  graph.removeVertex("YFS");
  graph.removeVertex("GKA");

  //removing the same set
  graph.removeVertex("LAE");
  graph.removeVertex("MAG");
  graph.removeVertex("HGU");
  graph.removeVertex("SBY");
  graph.removeVertex("YMM");
  graph.removeVertex("HZK");
  graph.removeVertex("YFS");
  graph.removeVertex("GKA");

  //testing for removal
  REQUIRE( graph.findVertex("GKA") == NULL  );
  REQUIRE( graph.findVertex("MAG") == NULL  );
  REQUIRE( graph.findVertex("HGU") == NULL  );
  REQUIRE( graph.findVertex("LAE") == NULL  );
  REQUIRE( graph.findVertex("HZK") == NULL  );
  REQUIRE( graph.findVertex("YFS") == NULL  );
  REQUIRE( graph.findVertex("YMM") == NULL  );
  REQUIRE( graph.findVertex("SBY") == NULL  );

  //edges should also be removed
  REQUIRE( graph.findEdge("HGULAE") == NULL  );
  REQUIRE( graph.findEdge("LAEHGU") == NULL  );
  REQUIRE( graph.findEdge("GKAHGU") == NULL  );

  REQUIRE( graph.findEdge("YFSSBY") == NULL  );
  REQUIRE( graph.findEdge("SBYYMM") == NULL  );
  REQUIRE( graph.findEdge("YMMHGU") == NULL  );
  REQUIRE( graph.findEdge("HGUYMM") == NULL  );
}
