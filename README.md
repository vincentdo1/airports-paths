# Transportation Network Analyzer
This tool analyzes a given spatial network and analyzes all possible shortest paths using Floyd–Warshall (FW) between nodes as well as the Betweenness Centrality (BC), which finds the importance of each node of the given network. As an example, we used the OpenFlights data set to see the importance of airports all around the globe. We normalized the BC output from 0 to 1 which allows for an easier understanding of the importance of each node.
## Important Files
We have four directories (catch, data, tests, and results) where data contains Openflights data and test data and tests are the data. The tests directory contains the Catch test suites plus an HTTP integration test:
- test.cpp: assures that the Adjlist class is implemented correctly and its basic functions work... The distance() function was tested by comparing the output of distance() with the known distance between two airports. Functions such as insertVertex() and insertEdge() were tested by manually inserting the vertex/edge then using our find() function to assert that the vertex/edge existed. Functions such as removeVertex() and removeEdge() were tested by manually inserting the vertex/edge then removing them. The find() function was then used to assert that the vertex/edge did not exist. In an attempt to produce errors, actions such as inserting/removing the same vertex/edge in different orders were executed.
- testBFS.cpp: checks BFS traversal on hand-built graphs
- test_alg.cpp: checks the implementation of FW and BC and works as expected with custom graphs
- testRouting.cpp: checks BFSPath (fewest hops) and DijkstraPath (shortest distance)
- testPool.cpp: checks the bounded thread pool, including overload shedding
- api_test.sh: an HTTP integration test that starts the server and checks every endpoint (run via CTest)

The results directory holds the normalized BC values for the data sets containing 500 and 1000 airports.\
The source code is structured with three different files:
- AdjList.h: header file that outlines all methods and member variables
- AdjList.cpp: implements basic methods to create and edit the graph implementation
- Algorithms.cpp: implements FW and BC as well as helper methods for testing

## Building and Testing
The canonical build is CMake with CTest (C++20). Presets are defined in `CMakePresets.json`:
```
cmake --preset debug          # or: release, asan-ubsan
cmake --build --preset debug
ctest --preset debug
```
The server binary is `build/debug/server` (or `build/release/server`); it listens on `:8080` and is configured through environment variables (see the HTTP API section below).

## Commands for Test Cases (legacy Makefile)
In addition to CMake, the original Makefile still builds each target individually. Run the following commands in terminal:\
make – compiles main.cpp\
make test – compiles test/test.cpp\
make test_alg – compiles test/test_alg.cpp\
make testBFS – compiles test/testBFS.cpp\
make testRouting – compiles tests/testRouting.cpp\
make testPool – compiles tests/testPool.cpp\
./main – runs main.cpp\
./test - runs test.cpp\
./test_alg – runs test_alg.cpp\
./testBFS – runs testBFS\
./testRouting – runs testRouting\
./testPool – runs testPool

## HTTP API
The project also includes a small HTTP/JSON service (`server.cpp`) that answers route, airport, and centrality queries over the loaded graph. Build and run it with:
```
make server
./server
```
By default it loads the 500-airport data set and listens on port 8080. Everything is configurable through environment variables: `PORT` (or `AIRPORT_PORT`), `AIRPORT_THREADS`, `AIRPORT_MAXQUEUE`, `AIRPORT_NODES`, `AIRPORT_EDGES`, `AIRPORT_CENTRALITY`, and `AIRPORT_CORS_ORIGIN`. It refuses to start if a required data file is missing.

Endpoints (all `GET`):
- `/healthz` – liveness check
- `/readyz` – readiness (the graph has loaded)
- `/api/v1/airports/{code}` – airport metadata, e.g. `/api/v1/airports/ORD`
- `/api/v1/routes?source=ORD&destination=NRT&mode=hops` – fewest-hop route (BFS)
- `/api/v1/routes?source=ORD&destination=NRT&mode=distance` – shortest-distance route (Dijkstra)
- `/api/v1/network/central-airports?limit=20` – most central airports, from the precomputed betweenness values

For example:
```
curl "http://localhost:8080/api/v1/routes?source=PEK&destination=JFK&mode=distance"
```
```json
{"source":"PEK","destination":"JFK","mode":"distance","algorithm":"dijkstra","path":["PEK","JFK"],"hops":1,"distanceKm":10972.2,"coordinates":[{"code":"PEK","lat":40.0801,"lng":116.585},{"code":"JFK","lat":40.6398,"lng":-73.7789}]}
```

The server reads the graph once at startup and then only reads it, so a fixed pool of worker threads answers requests concurrently without locking. The job queue is bounded and returns `503` when the backlog is full, and a total header-read deadline keeps a slow client from tying up a worker. The HTTP and JSON are written by hand to keep the dependency surface at zero and make the networking explicit; a production deployment would sit on a vetted HTTP/JSON library.

Betweenness centrality is expensive to compute (an informal timing was several minutes over the 500-airport set — not a rigorous benchmark), so it is produced offline by `main.cpp` into `results/` and served from there rather than recomputed per request.

### Docker / deployment
A multi-stage `Dockerfile` builds a slim, non-root image:
```
docker build -t airport-routing .
docker run --rm -p 8080:8080 airport-routing
```
`fly.toml` deploys it to Fly.io behind HTTPS (required, since the HTTPS GitHub Pages client can't call an HTTP API). The client points at the API through `REACT_APP_API_URL` at build time — see `ARCHITECTURE.md` for the full deploy flow.

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
## Interactive Display:
View the global model of the 500 airports and their paths here: https://vincentdo1.github.io/airports-paths/. The red airports represent the top 10 airports processed in the Betweenness Centrality Algorithm.
