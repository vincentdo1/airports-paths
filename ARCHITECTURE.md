# Architecture and Baseline Notes

This document describes how the airport routing service fits together and records
its build, test, and deployment setup. It is a map for anyone picking the project
up, not a formal specification.

## What the project is

The core is a directed graph of airports (nodes) and routes (edges) built from the
OpenFlights data set. On top of the original graph algorithms (Floyd–Warshall and
betweenness centrality) the project adds two per-query routing algorithms and a
small HTTP/JSON service that answers route, airport, and centrality queries. The
React/WebGL page in `visual-graph/` visualizes the network and can query the live
API. Dataset provenance and licensing are in `data/README.md`.

## Layout

| Piece | Files | Role |
| --- | --- | --- |
| Graph core | `AdjList.h`, `AdjList.cpp` | graph representation, load, insert/find/remove, distance |
| Algorithms | `Algorithms.cpp` | Floyd–Warshall, betweenness centrality, `BFSPath`, `DijkstraPath` |
| Thread pool | `ThreadPool.h`, `ThreadPool.cpp` | fixed workers, bounded queue |
| HTTP service | `server.cpp` | sockets, request parsing, JSON responses |
| Offline tool | `main.cpp` | regenerates betweenness centrality into `results/` |
| Build | `CMakeLists.txt`, `CMakePresets.json`, `Makefile` | CMake/CTest (canonical) + a legacy Makefile |
| Container | `Dockerfile`, `.dockerignore`, `fly.toml` | multi-stage image + Fly.io deploy config |
| Tests | `tests/` | Catch suites + an HTTP integration test |
| Data | `data/`, `results/` | OpenFlights nodes/edges and precomputed centrality |

## Graph core and ownership

- `AdjList` stores vertices and edges as `std::list<...*>` of heap nodes keyed by
  string airport code. Edges are directed: a vertex's `edges` list is its outgoing
  edges and `asEnd` is its incoming edges.
- The graph is built once from the node/edge files and then only read. It is
  non-copyable — the copy constructor and assignment operator are deleted (the
  originals were broken: the copy constructor recursed into itself and assignment
  left the target unchanged).
- Ownership caveat: the containers and `findVertex()` are public and return mutable
  pointers, so "read-only after load" is a convention the server relies on, not a
  compiler-enforced guarantee. Nothing on the request path mutates the graph, which
  is why the worker threads can share it without locking.

## Algorithms

| Algorithm | Location | Cost | Used by |
| --- | --- | --- | --- |
| `BFSPath` | `Algorithms.cpp` | O(V + E) | `/routes?mode=hops` |
| `DijkstraPath` | `Algorithms.cpp` | O(E log V) | `/routes?mode=distance` |
| `FWAlgorithm` | `Algorithms.cpp` | O(V³) | offline / tests |
| `BCAlgorithm` | `Algorithms.cpp` | ~O(V⁴) as written | offline (`main.cpp`) |

Betweenness centrality is expensive. An informal timing of a single pass over the
500-airport set was on the order of several minutes (~450 s, one run on a laptop —
not a controlled benchmark). The service therefore never computes it per request;
`main.cpp` generates it offline into `results/`, and `/network/central-airports`
serves those stored values.

## HTTP service and concurrency

- `server.cpp` opens a listening socket (Winsock on Windows, BSD sockets elsewhere),
  accepts connections on the main thread, and hands each one to a `ThreadPool`.
- The pool has a fixed number of workers and a bounded queue. When the queue is full
  `submit()` returns false and the server answers `503` instead of queuing without
  limit. A per-socket read timeout keeps a slow client from tying up a worker, and
  request headers larger than the cap are rejected with `413`.
- The graph is read-only after load, so workers read it concurrently without locks;
  only the pool's own job queue is mutex-guarded.
- Configuration comes from the environment and is validated at startup — a malformed
  or out-of-range value aborts rather than wrapping to nonsense: `PORT` /
  `AIRPORT_PORT`, `AIRPORT_THREADS`, `AIRPORT_MAXQUEUE`, `AIRPORT_NODES`,
  `AIRPORT_EDGES`, `AIRPORT_CENTRALITY`. Startup also fails fast if the dataset loads
  no airports, so a misconfigured deploy never appears healthy.
- Shutdown is graceful: `SIGINT`/`SIGTERM` closes the listener, and the pool drains
  its queue and joins every worker before the process exits.

Endpoints: `GET /healthz` (liveness), `GET /readyz` (graph loaded),
`GET /api/v1/airports/{code}`, `GET /api/v1/routes`,
`GET /api/v1/network/central-airports`.

## Building and testing

The canonical build is CMake + CTest, C++20. Presets are in `CMakePresets.json`.

```
cmake --preset debug           # or: release, asan-ubsan
cmake --build --preset debug
ctest --preset debug           # Catch suites + the HTTP integration test
```

Sanitizers (AddressSanitizer + UndefinedBehaviorSanitizer):

```
cmake --preset asan-ubsan
cmake --build --preset asan-ubsan
ctest --test-dir build/asan-ubsan --output-on-failure
```

Test suites (Catch2 v2.13.10, vendored under `catch/`):

- `test.cpp` — 145 assertions — construction, distance, insert/find/remove
- `test_alg.cpp` — 248 assertions — Floyd–Warshall and betweenness centrality
- `testBFS.cpp` — 161 assertions — BFS traversal on hand-built graphs
- `testRouting.cpp` — 18 assertions — `BFSPath` and `DijkstraPath`
- `testPool.cpp` — thread pool behaviour including overload shedding
- `tests/api_test.sh` — HTTP integration test: starts the server and checks every
  endpoint, including the 413 / readiness / validation cases (registered with CTest
  on Unix-like systems)

The legacy `Makefile` still builds each target individually (`make`, `make test`, …)
for the original workflow, but CMake is canonical.

## Continuous integration

`.github/workflows/ci.yml` runs on Linux:

- **build-and-test** — configure, build, and `ctest` with the debug preset.
- **sanitizers** — routing and pool tests under ASan/UBSan.
- **docker** — builds the image, runs the container, issues a real route request,
  and stops it.

## Deployment

A multi-stage `Dockerfile` builds the server from the release preset into a slim,
non-root runtime image containing only the binary and the `data/` + `results/`
files. The server honours `$PORT`. `fly.toml` deploys it to Fly.io behind HTTPS —
required, because the HTTPS GitHub Pages frontend cannot call an HTTP API (mixed
content). Point the frontend at the deployed URL with `REACT_APP_API_URL` at build
time.

## Deliberate scope

The tooling is modernized (C++20, CMake/CTest, CI, Docker), but the **hand-written
socket server and thread pool are kept as the centerpiece** rather than replaced by
a framework. The project intentionally does not adopt Boost.Asio/Beast, an immutable
graph store, a route cache, or a metrics/observability stack. The HTTP and JSON
handling is written by hand to make the networking and concurrency explicit; a
production service would sit on a vetted HTTP/JSON library.

## Known limitations

- **I/O and compute share the worker pool.** A worker performs the blocking `recv`,
  the route computation, and the `send` for its connection, so a burst of slow
  clients can occupy the workers (bounded by the read timeout). Behind a buffering
  reverse proxy or platform edge this is mitigated; a fuller fix — separating
  asynchronous I/O from a dedicated compute pool — is a possible future step.
- The bounded queue limits queued *connections*; concurrent route *computations* are
  bounded by the worker count.
- The graph is read-only by convention, not enforced immutability.
- The hand-written HTTP layer covers only the GET subset the API needs.
- One data set is loaded per process; switch sets with `AIRPORT_NODES` /
  `AIRPORT_EDGES` / `AIRPORT_CENTRALITY`.
- Centrality values are only as fresh as `results/`; regenerate them with `main.cpp`.
