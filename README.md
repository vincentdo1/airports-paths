# Transportation Network Analyzer

This project analyzes directed spatial networks with breadth-first search, Dijkstra,
Floyd–Warshall, and betweenness centrality (BC). The included OpenFlights snapshot
powers a C++ routing API and a React/WebGL globe.

## Project layout

- `AdjList.{h,cpp}` — graph storage and operations
- `Algorithms.cpp` — traversal, shortest-path, and centrality algorithms
- `server.cpp`, `ThreadPool.{h,cpp}` — HTTP API and bounded worker pool
- `main.cpp` — offline BC executable
- `data/`, `results/` — input snapshots and precomputed centrality values
- `tests/` — Catch suites and the HTTP integration test
- `visual-graph/` — interactive frontend

## Building and Testing

The canonical build is CMake with CTest (C++20). Presets are in `CMakePresets.json`:

```sh
cmake --preset debug          # or: release, asan-ubsan
cmake --build --preset debug
ctest --preset debug
```

Run the debug server from the repository root so its default data paths resolve:

```sh
./build/debug/server
```

On Windows, use `.\build\debug\server.exe`. The original Makefile remains available
for the legacy per-target workflow.

## HTTP API

`server.cpp` answers route, airport, and centrality queries over the loaded graph.
It loads the 500-airport set on port 8080 by default. Configuration comes from
`PORT` (or `AIRPORT_PORT`), `AIRPORT_THREADS`, `AIRPORT_MAXQUEUE`, `AIRPORT_NODES`,
`AIRPORT_EDGES`, `AIRPORT_CENTRALITY`, and `AIRPORT_CORS_ORIGIN`. It refuses to
start if a required data file is missing.

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

The graph is loaded once and shared by workers without graph locks. That read-only
state is a convention, not compiler-enforced: `AdjList` still exposes mutable node
pointers, although request handlers do not mutate them. Each worker handles both
socket I/O and route computation, so the header deadline limits request stalls and
the bounded queue limits backlog; a full queue returns `503`.

BC is too expensive to compute per request, so the endpoint serves precomputed files
checked into `results/`. These are snapshots and do not update automatically when the
dataset changes.

### Docker and deployment

```
docker build -t airport-routing .
docker run --rm -p 8080:8080 airport-routing
```
`fly.toml` deploys that image to Fly.io behind HTTPS, which GitHub Pages requires:
an HTTPS page cannot call an HTTP API. Point the frontend at the deployed URL with
`REACT_APP_API_URL` at build time.

## Origins and credits

This repository began as a CS 225 team project by Vincent Do, Jeremy Lee, Andrew Li,
and Tyler Shu. The [final project video](https://www.youtube.com/watch?v=P4iwqSvMmCA)
shows the original version.

## Interactive Display

Explore the [interactive globe](https://vincentdo1.github.io/airports-paths/). Gold
markers identify the ten airports with the highest checked-in BC scores.
