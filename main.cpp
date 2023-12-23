
#include "AdjList.h"
#include <fstream>

int main() {
  AdjList graph("data/nodes500.txt", "data/edges500.txt");
  std::cout << "graph loaded " << graph.vertexList.size() << '\n';
  graph.BFSTraversal();
  std::map<std::string, double> bc = graph.BCAlgorithm();
  std::ofstream outFile("OpenFlightsBC500.txt");
  for(auto itr = bc.begin(); itr != bc.end(); itr++){
      outFile << itr->first << ": " << itr->second << '\n';
  }
  outFile.close();
  return 0;
}
