# Transportation Network Analyzer
This tool analyzes a given spatial network and analyzes all possible shortest paths using Floyd–Warshall (FW) between nodes as well as the Betweenness Centrality (BC), which finds the importance of each node of the given network. As an example, we used the OpenFlights data set to see the importance of airports all around the globe. We normalized the BC output from 0 to 1 which allows for an easier understanding of the importance of each node.

## Important Files
Four directories hold the inputs and outputs: `data` (OpenFlights and test data), `tests`, `results`, and `catch`.

The source code is structured with three different files:
- AdjList.h: header file that outlines all methods and member variables
- AdjList.cpp: implements basic methods to create and edit the graph implementation
- Algorithms.cpp: implements FW, BC, and the per-query BFSPath and DijkstraPath

The server, its worker pool, and the offline BC generator live in `server.cpp`, `ThreadPool.{h,cpp}`, and `main.cpp`.

The tests directory contains the Catch suites plus an HTTP integration test:
- test.cpp: assures that the AdjList class is implemented correctly and its basic functions work. distance() was tested against known distances between two airports. insertVertex()/insertEdge() were tested by inserting then asserting with find(); removeVertex()/removeEdge() by inserting, removing, then asserting find() no longer sees them. In an attempt to produce errors, inserting and removing the same vertex/edge in different orders was executed.
- testBFS.cpp: checks BFS traversal on hand-built graphs
- test_alg.cpp: checks the implementation of FW and BC against custom graphs
- testRouting.cpp: checks BFSPath (fewest hops) and DijkstraPath (shortest distance)
- testPool.cpp: checks the bounded thread pool, including overload shedding
- api_test.sh: starts the server and checks every endpoint (run via CTest)

The results directory holds the normalized BC values for the data sets containing 500 and 1000 airports.

## Building and Testing
The canonical build is CMake with CTest (C++20). Presets are in `CMakePresets.json`:
```
cmake --preset debug          # or: release, asan-ubsan
cmake --build --preset debug
ctest --preset debug
```
The original Makefile still builds each target individually — `make`, `make server`, `make test`, `make test_alg`, `make testBFS`, `make testRouting`, `make testPool` — and each produces a binary of the same name.

## HTTP API
`server.cpp` is a small HTTP/JSON service answering route, airport, and centrality queries over the loaded graph:
```
make server
./server
```
It loads the 500-airport set on port 8080 by default. Everything is configurable through the environment: `PORT` (or `AIRPORT_PORT`), `AIRPORT_THREADS`, `AIRPORT_MAXQUEUE`, `AIRPORT_NODES`, `AIRPORT_EDGES`, `AIRPORT_CENTRALITY`, `AIRPORT_CORS_ORIGIN`. It refuses to start if a required data file is missing.

Endpoints (all `GET`):
- `/healthz` – liveness
- `/readyz` – readiness (the graph has loaded)
- `/api/v1/airports/{code}` – airport metadata, e.g. `/api/v1/airports/ORD`
- `/api/v1/routes?source=ORD&destination=NRT&mode=hops` – fewest-hop route (BFS)
- `/api/v1/routes?source=ORD&destination=NRT&mode=distance` – shortest-distance route (Dijkstra)
- `/api/v1/network/central-airports?limit=20` – most central airports, from the precomputed BC values

```
curl "http://localhost:8080/api/v1/routes?source=PEK&destination=JFK&mode=distance"
```
```json
{"source":"PEK","destination":"JFK","mode":"distance","algorithm":"dijkstra","path":["PEK","JFK"],"hops":1,"distanceKm":10972.2,"coordinates":[{"code":"PEK","lat":40.0801,"lng":116.585},{"code":"JFK","lat":40.6398,"lng":-73.7789}]}
```

The graph is read once at startup and then only read, so a fixed pool of workers answers requests without locking. The job queue is bounded and returns `503` when the backlog fills, and a header-read deadline keeps a slow client from tying up a worker. HTTP and JSON are written by hand to keep the dependency surface at zero; a production deployment would sit on a vetted library instead.

BC is far too expensive to compute per request, so `main.cpp` generates it offline into `results/` and the endpoint serves those values.

### Docker and deployment
```
docker build -t airport-routing .
docker run --rm -p 8080:8080 airport-routing
```
`fly.toml` deploys that image to Fly.io behind HTTPS, which GitHub Pages requires: an HTTPS page can't call an HTTP API. Point the frontend at the deployed URL with `REACT_APP_API_URL` at build time.

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

## Interactive Display
View the global model of the 500 airports and their paths here: https://vincentdo1.github.io/airports-paths/. The red airports represent the top 10 airports processed in the Betweenness Centrality Algorithm.
