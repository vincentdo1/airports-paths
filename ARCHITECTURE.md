# Architecture and Baseline Notes

This document describes how the airport routing service fits together and records
the baseline state of the repository when the HTTP API was added. It is a map for
anyone picking the project up, not a formal specification.

## What the project is

The core is a directed graph of airports (nodes) and routes (edges) built from the
OpenFlights data set. On top of the original graph algorithms (Floyd–Warshall and
betweenness centrality) the project adds two per-query routing algorithms and a
small HTTP/JSON service that answers route, airport, and centrality queries. The
existing React/WebGL page in `visual-graph/` visualizes the network.

## Layout

| Piece | Files | Role |
| --- | --- | --- |
| Graph core | `AdjList.h`, `AdjList.cpp` | graph representation, load, insert/find/remove, distance |
| Algorithms | `Algorithms.cpp` | Floyd–Warshall, betweenness centrality, `BFSPath`, `DijkstraPath` |
| Thread pool | `ThreadPool.h`, `ThreadPool.cpp` | fixed workers, bounded queue |
| HTTP service | `server.cpp` | sockets, request parsing, JSON responses |
| Offline tool | `main.cpp` | regenerates betweenness centrality into `results/` |
| Tests | `tests/` | Catch test suites |
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

Betweenness centrality is expensive: one pass over the 500-airport set measured at
about **453 seconds**. The service therefore never computes it per request.
`main.cpp` generates it offline into `results/`, and the
`/network/central-airports` endpoint serves those stored values.

## HTTP service and concurrency

- `server.cpp` opens a listening socket (Winsock on Windows, BSD sockets elsewhere),
  accepts connections on the main thread, and hands each one to a `ThreadPool`.
- The pool has a fixed number of workers and a bounded queue. When the queue is
  full `submit()` returns false and the server answers `503` instead of queuing
  without limit. A per-socket read timeout keeps a slow client from tying up a
  worker indefinitely.
- The graph is immutable after load, so workers read it concurrently without locks;
  only the pool's own job queue is mutex-guarded.
- Configuration is read from the environment, with defaults matching the repo data:
  `AIRPORT_PORT`, `AIRPORT_THREADS`, `AIRPORT_MAXQUEUE`, `AIRPORT_NODES`,
  `AIRPORT_EDGES`, `AIRPORT_CENTRALITY`.

Endpoints: `GET /healthz`, `GET /api/v1/airports/{code}`, `GET /api/v1/routes`,
`GET /api/v1/network/central-airports`.

## Baseline build and test status

Built and tested with g++ 16.1 (MinGW-w64, UCRT) on Windows. The code is C++11
(`-std=gnu++11`); `clang++` and `make` from the original EWS setup are not required,
though the Makefile still honours `make CXX=clang++`.

- `test.cpp` — 145 assertions — construction, distance, insert/find/remove
- `test_alg.cpp` — 248 assertions — Floyd–Warshall and betweenness centrality
- `testBFS.cpp` — BFS traversal (prints for manual inspection)
- `testRouting.cpp` — 18 assertions — `BFSPath` and `DijkstraPath` (added)
- `testPool.cpp` — thread pool behaviour including overload shedding (added)

Toolchain notes found while establishing the baseline:

- `M_PI` is not defined under strict `-std=c++0x` on this libc; `-std=gnu++11`
  (same C++11 language, GNU extensions) resolves it.
- `test.cpp` uses `== NULL` inside Catch's expression templates, which modern g++
  rejects (pointer vs int). The other suites are unaffected.
- A pre-existing `removeEdge()` bug — it erased an iterator from `edges` while
  iterating `asEnd` — aborted teardown under this runtime's checked containers and
  is now fixed.

## Deliberate scope

To keep the enhancement in the spirit of the existing project, it intentionally
does **not** add Boost, CMake, a package manager, Docker, or a C++20 migration, and
does not implement operational endpoints (metrics, readiness) beyond a liveness
check. The HTTP and JSON handling is written by hand to keep the dependency surface
at zero and to make the networking and concurrency explicit; a production service
would sit on a vetted HTTP/JSON library instead.

## Known limitations

- The graph is read-only by convention, not enforced immutability.
- The hand-written HTTP layer covers only the GET subset the API needs.
- One data set is loaded per process; switch sets with the `AIRPORT_NODES` /
  `AIRPORT_EDGES` / `AIRPORT_CENTRALITY` environment variables.
- Centrality values are only as fresh as `results/`; regenerate them with `main.cpp`.
