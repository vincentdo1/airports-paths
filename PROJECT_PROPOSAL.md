---------------------------------FINAL PROJECT PROPOSAL - TEAM JUSTIN FIELDS---------------------------------

**LEADING QUESTION**\
  The invention and discovery of graph algorithms and their applications has presented a new opportunity for     
  individuals around the world to organize, access, and interpret their data. Given a dataset and a code base\
  that implements graph algorithms, we can hope to establish connections and relations between given points and\
  sets of data, providing the opportunity to draw patterns between data. Specifically, we are hoping to find\
  routes between airports based on distance, classifying the most important airports based on average distance\
  and throughnesss. 
  
**DATASET ACQUISITION AND PROCESSING**\
  Datasets are widely available online. For our final project, we will be looking at the OpenFlights Data. \
  Specifically, we will be using the Routes database provided by OpenFlights. The database includes information\
  about Airlines, as well as their soure airport and destination airport. We can utilise this data\
  and map it onto the provided Airports database, also given by OpenFlights, to visually represent\
  the airports that have the highest traffic.
  
  The data is formatted via metadata, and location to and from the airport. Thus, our nodes will be airports\
  and our edges will be the routes taken by airlines to and from sources and destinations. The option to add\
  weights to our edges based on the traffic of each route is also available - this would allow us to interpret\
  which routes are more important. After parsing the text, using the format provided by the website, we will\
  be able to graph out the connections between airports. This could also open up the opportunity to create\
  a shortest route traversal.
  
  Errors in the datasets may occur primarily when data is missing - i.e. route information for a source is\
  missing or the data is incomplete. Upon encountering an error, the code will either drop the route entirely\
  or attempt to infer its missing information. In other words, the code will first check if the route or format\
  of the data is valid, and then decide whether or not it will keep or process the data.
  
  The link to the datasets is included here for reference: https://openflights.org/data.html
  
**GRAPH ALGORITHMS**\
    As per guidelines, the code base will implement a Graph Traversal, as well as two graphs algorithms.\
    The graph traversal will be a BFS traversal that goes from shortest to furthest distance. It will be an\
    iterator that accomplishes such. We will also use the Betweeness Centrality algorithm, which will\
    essentially tell us the most important airport. Its input will be the whole graph, and its output will\
    be a list of nodes, sorted from greatest to least centrality. Our target runtime will be O(n^3).\
    Finally, our code base will also implement Dijkstra's Algorithm for Average Path Length, or\
    the shortest path. It takes in a node as the input, and outputs the average path length to all the\
    other nodes, essentially outlining a node's average distance from other nodes. Our target runtime will\
    be O(n * (E log(V) + V log(E))).
  
**TIMELINE**
    The final project timeline is outlined below. Although the timelines aren't particularly strict, \
    the timing of each deadling should act as a guideline for gauging the project's progress, and\
    whether more or less work needs to be done. The following outlines when each item should be\
    completed.\
    **DATA ACQUISITION**: Monday, 11/8 - Thursday, 11/11\
      Data acquisition consists primarily of the collection of datasets, as well as the analyzation\
      of the formats of provided datasets. Datasets will need to be reviewed and potentially organized\
      into usable sets of information.\
    **DATA PROCESSING**: Friday, 11/12 - Friday, 11/19\
      The methods of processing data should be take at least a week to complete. The data processing\
      will likely stem from text processing and numerical recognition, given the format of the files\
      comes in text. \
    **COMPLETION OF INDIVIDUAL ALGORITHMS**: Saturday, 11/20 - Tuesday, 12/7\
      The completion of the algorithms should be fairly straightforward - members will need to \
      reformat the algorithms to be applicable to our datasets. Time is allotted outside of fall break\
      as well as during fall break if necessary, though the goal is not to work on the project (at least\
      extensively) over fall break to preserve the integrity and mental health of the group members.\
    **PRODUCTION OF FINAL DELIVERABLES**: Wednesday, 12/8 - Friday, 12/10\
      The production of final deliverables should be completed ahead of time of the project due date.\
      Completion ahead of the deadline allows the group room for errors, providing time for the group\
      to fix any issues or errors that may come up.
