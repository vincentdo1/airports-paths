# Transportation Network Analyzer
This tool analyzes a given spatial network and analyzes all possible shortest paths using Floyd–Warshall (FW) between nodes as well as the Betweenness Centrality (BC), which finds the importance of each node of the given network. As an example, we used the OpenFlights data set to see the importance of airports all around the globe. We normalized the BC output from 0 to 1 which allows for an easier understanding of the importance of each node.
## Important Files
We have four directories (catch, data, tests, and results) where data contains Openflights data and test data and tests are the data. The tests directory contains three different tests:
- test.cpp: assures that the Adjlist class is implemented correctly and its basic functions work... The distance() function was tested by comparing the output of distance() with the known distance between two airports. Functions such as insertVertex() and insertEdge() were tested by manually inserting the vertex/edge then using our find() function to assert that the vertex/edge existed. Functions such as removeVertex() and removeEdge() were tested by manually inserting the vertex/edge then removing them. The find() function was then used to assert that the vertex/edge did not exist. In an attempt to produce errors, actions such as inserting/removing the same vertex/edge in different orders were executed.
- testBFS.cpp: prints the BFS of graphs for us to check manually
- test_alg.cpp: checks the implementation of FW and BC and works as expected with custom graphs

The results directory holds the normalized BC values for the data sets containing 500 and 1000 airports.\
The source code is structured with three different files:
- AdjList.h: header file that outlines all methods and member variables
- AdjList.cpp: implements basic methods to create and edit the graph implementation
- Algorithms.cpp: implements FW and BC as well as helper methods for testing

In addition we have a Makefile to allow for easy compilation of test cases.
## Commands for Test Cases
Run the following commands in terminal:\
make – compiles main.cpp\
make test – compiles test/test.cpp\
make test_alg – compiles test/test_alg.cpp\
make testBFS – compiles test/testBFS.cpp\
./main – runs main.cpp\
./test - runs test.cpp\
./test_alg – runs test_alg.cpp\
./testBFS – runs testBFS
## Running with OpenFlights example
The graph can be loaded as such:
```
AdjList graph("data/origNodes.txt", "data/origEdges.txt");
```
where the first parameter is the path to nodes and the second parameter is the path to the edges. The file format should be three columns separated by spaces of text files. The node file should have a string ID in the first column and latitude and longitude in the second and third columns. The edge file should have string ID in the first column and the start and end node ID in the second and third column.\
Next:
```
std::map<std::string, double> bc = graph.BCAlgorithm();
```
which will get a map of all the BC values of every airport!
## Results
The outputted BC values are viewable in the results directory which contains the BC values of the 500 and 1000 most visited airports. These can be viewed in the OpenFlightsBC1000.txt and OpenFlightsBC500.txt or in their sorted versions (descending).
## Video
Link to final project video on [youtube](https://www.youtube.com/watch?v=P4iwqSvMmCA&ab_channel=TylerShu)
