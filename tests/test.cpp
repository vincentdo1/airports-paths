#include "../AdjList.h"
#include "../catch/catch.hpp"
#include <iostream>


// Adjlist default constructor
TEST_CASE("AdjList's custom constructor works", "[weight=1]") {
  AdjList graph("data/nodes.txt", "data/edges.txt");
  //all vertices should exist... testing for some
  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );
  REQUIRE( graph.findVertex("HGU") != nullptr  );
  REQUIRE( graph.findVertex("LAE") != nullptr  );
  REQUIRE( graph.findVertex("YMM") != nullptr  );

  //all edges should exist... testing for some
  REQUIRE( graph.findEdge("GKAMAG") != nullptr  );
  REQUIRE( graph.findEdge("MAGGKA") != nullptr  );
  REQUIRE( graph.findEdge("HGUGKA") != nullptr  );
  REQUIRE( graph.findEdge("MAGHGU") != nullptr  );
  REQUIRE( graph.findEdge("LAEMAG") != nullptr  );
  REQUIRE( graph.findEdge("LAEHGU") != nullptr  );
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
  REQUIRE( graph.findVertex("GKA") == nullptr  );
  REQUIRE( graph.findVertex("MAG") == nullptr  );

  //testing weird inputs
  REQUIRE( graph.findVertex("UUU") == nullptr  );
  REQUIRE( graph.findVertex("   ") == nullptr  );
  REQUIRE( graph.findVertex("") == nullptr  );
  REQUIRE( graph.findVertex("!@#") == nullptr  );
  REQUIRE( graph.findVertex("!@#$%^&*()") == nullptr  );
  REQUIRE( graph.findVertex("ADA") == nullptr  );

}

TEST_CASE("AdjList's findEdge()  test", "[weight=1]") {
  AdjList graph;

  //Edge should not exist!
  REQUIRE( graph.findEdge("GKAMAG") == nullptr  );

  //testing weird inputs
  REQUIRE( graph.findEdge("      ") == nullptr  );
  REQUIRE( graph.findEdge("") == nullptr  );
  REQUIRE( graph.findEdge("NNNNNN") == nullptr  );
  REQUIRE( graph.findEdge("QWERTY") == nullptr  );
  REQUIRE( graph.findEdge("QWERTYUIOP") == nullptr  );
  REQUIRE( graph.findEdge("!@#$%^&*()") == nullptr  );
  REQUIRE( graph.findEdge("ADADAD") == nullptr  );
}


TEST_CASE("AdjList's insertEdge() simple test", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  //Edge should not exist!
  REQUIRE( graph.findEdge("GKAMAG") == nullptr  );

  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  //edge should exist
  REQUIRE( graph.findEdge("GKAMAG") != nullptr  );
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
  REQUIRE( graph.findEdge("GKAMAG") != nullptr  );
  REQUIRE( graph.findEdge("MAGGKA") != nullptr  );
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
  REQUIRE( graph.findEdge("GKAMAG") != nullptr  );
  REQUIRE( graph.findEdge("MAGGKA") != nullptr  );
  REQUIRE( graph.findEdge("HGUGKA") != nullptr  );
  REQUIRE( graph.findEdge("MAGHGU") != nullptr  );
  REQUIRE( graph.findEdge("LAEMAG") != nullptr  );
  REQUIRE( graph.findEdge("LAEHGU") != nullptr  );

}

TEST_CASE("AdjList's insertVertex() simple test", "[weight=1]") {
  AdjList graph;
  //vertex should not exist!
  REQUIRE( graph.findVertex("GKA") == nullptr  );
  REQUIRE( graph.findVertex("MAG") == nullptr  );

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  //testing for incorrect latitudes and longs; remove line if unapplicable!!!
  graph.insertVertex("GKA",33,33);

  //vertex should exist!
  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );
}

TEST_CASE("AdjList's insertVertex() multiple test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("HGU", -5.826789855957031, 144.29600524902344);
  graph.insertVertex("LAE", -6.569803, 146.725977);

  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );
  REQUIRE( graph.findVertex("HGU") != nullptr  );
  REQUIRE( graph.findVertex("LAE") != nullptr  );
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

  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );
  REQUIRE( graph.findVertex("HGU") != nullptr  );
  REQUIRE( graph.findVertex("LAE") != nullptr  );
  REQUIRE( graph.findVertex("HZK") != nullptr  );
  REQUIRE( graph.findVertex("YFS") != nullptr  );
  REQUIRE( graph.findVertex("YMM") != nullptr  );
  REQUIRE( graph.findVertex("SBY") != nullptr  );
}

TEST_CASE("AdjList's removeEdge() simple test", "[weight=1]") {
  AdjList graph;
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  //Edge should not exist!
  REQUIRE( graph.findEdge("GKAMAG") == nullptr  );

  graph.insertEdge("GKAMAG", graph.findVertex("GKA"), graph.findVertex("MAG"));

  //edge should exist
  REQUIRE( graph.findEdge("GKAMAG") != nullptr  );

  //remove edge
  graph.removeEdge("GKAMAG");

  //edge should not exist
  REQUIRE( graph.findEdge("GKAMAG") == nullptr  );
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
  REQUIRE( graph.findEdge("GKAMAG") == nullptr  );
  REQUIRE( graph.findEdge("MAGGKA") == nullptr  );
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
  REQUIRE( graph.findEdge("GKAMAG") == nullptr  );
  REQUIRE( graph.findEdge("MAGGKA") == nullptr  );
  REQUIRE( graph.findEdge("LAEHGU") == nullptr  );
  REQUIRE( graph.findEdge("MAGHGU") == nullptr  );
  REQUIRE( graph.findEdge("LAEMAG") == nullptr  );
  REQUIRE( graph.findEdge("HGUGKA") == nullptr  );
}

TEST_CASE("AdjList's removeVertex() simple test", "[weight=1]") {
  AdjList graph;

  //insert vertices
  graph.insertVertex("GKA",-6.081689834590001,145.391998291);
  graph.insertVertex("MAG",-5.20707988739,145.789001465);

  graph.insertEdge("MAGGKA", graph.findVertex("MAG"), graph.findVertex("GKA"));

  //vertex should exist!
  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );

  graph.removeVertex("GKA");

  //vertex should be removed!
  REQUIRE( graph.findVertex("GKA") == nullptr  );

  graph.removeVertex("MAG");

  REQUIRE( graph.findVertex("MAG") == nullptr  );

  //edge should also be removed
  REQUIRE( graph.findEdge("MAGGKA") == nullptr  );
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

  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );
  REQUIRE( graph.findVertex("HGU") != nullptr  );
  REQUIRE( graph.findVertex("LAE") != nullptr  );

  //removing vertices
  graph.removeVertex("GKA");
  graph.removeVertex("LAE");
  graph.removeVertex("HGU");
  graph.removeVertex("MAG");


  REQUIRE( graph.findVertex("GKA") == nullptr  );
  REQUIRE( graph.findVertex("MAG") == nullptr  );
  REQUIRE( graph.findVertex("HGU") == nullptr  );
  REQUIRE( graph.findVertex("LAE") == nullptr  );

  //edges should also be removed
  REQUIRE( graph.findEdge("HGULAE") == nullptr  );
  REQUIRE( graph.findEdge("LAEHGU") == nullptr  );
  REQUIRE( graph.findEdge("GKAHGU") == nullptr  );
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


  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );
  REQUIRE( graph.findVertex("HGU") != nullptr  );
  REQUIRE( graph.findVertex("LAE") != nullptr  );
  REQUIRE( graph.findVertex("HZK") != nullptr  );
  REQUIRE( graph.findVertex("YFS") != nullptr  );
  REQUIRE( graph.findVertex("YMM") != nullptr  );
  REQUIRE( graph.findVertex("SBY") != nullptr  );

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
  REQUIRE( graph.findVertex("GKA") == nullptr  );
  REQUIRE( graph.findVertex("MAG") == nullptr  );
  REQUIRE( graph.findVertex("HGU") == nullptr  );
  REQUIRE( graph.findVertex("LAE") == nullptr  );
  REQUIRE( graph.findVertex("HZK") == nullptr  );
  REQUIRE( graph.findVertex("YFS") == nullptr  );
  REQUIRE( graph.findVertex("YMM") == nullptr  );
  REQUIRE( graph.findVertex("SBY") == nullptr  );

  //edges should also be removed
  REQUIRE( graph.findEdge("HGULAE") == nullptr  );
  REQUIRE( graph.findEdge("LAEHGU") == nullptr  );
  REQUIRE( graph.findEdge("GKAHGU") == nullptr  );

  //These assertions produce weird stuff
  REQUIRE( graph.findEdge("YFSSBY") == nullptr  );
  REQUIRE( graph.findEdge("SBYYMM") == nullptr  );
  REQUIRE( graph.findEdge("YMMHGU") == nullptr  );
  REQUIRE( graph.findEdge("HGUYMM") == nullptr  );
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


  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );
  REQUIRE( graph.findVertex("HGU") != nullptr  );
  REQUIRE( graph.findVertex("LAE") != nullptr  );
  REQUIRE( graph.findVertex("HZK") != nullptr  );
  REQUIRE( graph.findVertex("YFS") != nullptr  );
  REQUIRE( graph.findVertex("YMM") != nullptr  );
  REQUIRE( graph.findVertex("SBY") != nullptr  );

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
  REQUIRE( graph.findVertex("GKA") == nullptr  );
  REQUIRE( graph.findVertex("MAG") == nullptr  );
  REQUIRE( graph.findVertex("LAE") == nullptr  );
  REQUIRE( graph.findVertex("HZK") == nullptr  );
  REQUIRE( graph.findVertex("YFS") == nullptr  );
  REQUIRE( graph.findVertex("YMM") == nullptr  );
  REQUIRE( graph.findVertex("SBY") == nullptr  );

  //edges should also be removed
  REQUIRE( graph.findEdge("YFSSBY") == nullptr  );
  REQUIRE( graph.findEdge("SBYYMM") == nullptr  );
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


  REQUIRE( graph.findVertex("GKA") != nullptr  );
  REQUIRE( graph.findVertex("MAG") != nullptr  );
  REQUIRE( graph.findVertex("HGU") != nullptr  );
  REQUIRE( graph.findVertex("LAE") != nullptr  );
  REQUIRE( graph.findVertex("HZK") != nullptr  );
  REQUIRE( graph.findVertex("YFS") != nullptr  );
  REQUIRE( graph.findVertex("YMM") != nullptr  );
  REQUIRE( graph.findVertex("SBY") != nullptr  );

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
  REQUIRE( graph.findVertex("GKA") == nullptr  );
  REQUIRE( graph.findVertex("MAG") == nullptr  );
  REQUIRE( graph.findVertex("HGU") == nullptr  );
  REQUIRE( graph.findVertex("LAE") == nullptr  );
  REQUIRE( graph.findVertex("HZK") == nullptr  );
  REQUIRE( graph.findVertex("YFS") == nullptr  );
  REQUIRE( graph.findVertex("YMM") == nullptr  );
  REQUIRE( graph.findVertex("SBY") == nullptr  );

  //edges should also be removed
  REQUIRE( graph.findEdge("HGULAE") == nullptr  );
  REQUIRE( graph.findEdge("LAEHGU") == nullptr  );
  REQUIRE( graph.findEdge("GKAHGU") == nullptr  );

  REQUIRE( graph.findEdge("YFSSBY") == nullptr  );
  REQUIRE( graph.findEdge("SBYYMM") == nullptr  );
  REQUIRE( graph.findEdge("YMMHGU") == nullptr  );
  REQUIRE( graph.findEdge("HGUYMM") == nullptr  );
}
