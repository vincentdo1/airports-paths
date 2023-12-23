# Function Outputs
There are three functions that our project implemented, breadth first search (BFS), Floyd–Warshall's (FW), and Betweenness Centrality (BC). Looking at each function's output and expected output more closely:
- BFS:
- FW:
- BC:

# Discussion
  For BFS our implementation is successful considering the output of the BFS accurately printed every node in
in order of its traversal as well as providing a list of edges formatted to read clearly. In addition every edge was listed as either a cross or discovery edge which allows the user to see which edges are visited new nodes. These were also hand checked with test cases that were complex but smaller than the OpenFlights dataset allowing use to ensure that there were no issues with our implementation.\
  FW's implementation is also successful since it passed all of the test cases we had written for it. The
implementation and test cases were developed by two different people to ensure that there was no designing for the actual implementation. We also had extensive tests for both complicated and simple cases. Its output is a table of pairs with the first value being a double and the second being a vertex node pointer that will be the next node in the path to get to the target in the shortest distance.\
  For BC, the output of our implementation is best seen in the results directory. More specifically, looking at
the sorted text files for 500 and 1000 nodes. The size of the network had to be cut down since running BC (and by extension FW) took upwards of 16 hours to run. We cut down the network size by only selecting the top 500 and 1000 most visited airports world wide using this [site] (https://gettocenter.com/airports/top-100-airports-in-world/1000). This still got over 2/3s of the original edges using less than 1/6th of nodes meaning many of the edges were captured. These values were then sorted to better view the "most important" airport. These orders very roughly aligned with high traffic airports. The airports with lower traffic but have high BC values most likely solve something similar to the last mile problem. This can be seen with the Manila airport which presumable has many connections to smaller local airports nearby islands. As the number of airports increases, more airports like these will start to appear with a higher BC score.
