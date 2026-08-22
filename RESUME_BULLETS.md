# Résumé material — Airport Routing Service

Personal reference (NOT part of the project — don't commit this to the public repo).
Everything here is **backable**: verifiable from the code, the tests, or the live
deployment. Nothing is invented. Deliberately excluded: req/s, p99/latency,
"high-performance / scalable / production-grade," uptime, or user counts — none of
those are measured, so none are claimed.

Live: API https://airport-routing-vmd306.fly.dev · client https://vincentdo1.github.io/airports-paths

Suggested project header:
**Airport Routing Service** — C++20 · sockets · CMake/CTest · GitHub Actions · Docker · Fly.io · React

---

## Résumé bullets (pick 2–3; trim to taste)

**Concurrency / backpressure (your Option A):**
- Implemented bounded concurrency in a hand-written C++20 HTTP service using a fixed
  worker pool and a capacity-limited request queue, shedding load with HTTP 503
  beyond capacity; added a total request deadline and header-size limits, with
  integration tests covering deadlines, oversized requests, overload shedding, and
  health/readiness endpoints.

**Service / algorithms:**
- Built a C++20 HTTP/JSON routing service over an in-memory airport graph (500 nodes,
  ~16k directed edges), serving minimum-hop (BFS) and minimum-distance (Dijkstra)
  routes plus precomputed betweenness-centrality network statistics.

**Lockless reads / ownership:**
- Loaded the graph once into a read-only in-memory store so worker threads serve
  concurrent reads without locking, confining mutable shared state to the pool's
  mutex-guarded queue.

**Build + CI + sanitizers (infra):**
- Established a CMake/CTest build with presets and GitHub Actions CI running unit +
  HTTP integration tests, AddressSanitizer/UndefinedBehaviorSanitizer, a multi-stage
  Docker image build, and a container smoke test that issues a real route request.

**Containerization + deployment (infra):**
- Containerized the service (multi-stage, non-root, slim runtime image) and deployed
  it on Fly.io over HTTPS, with a React/WebGL client on GitHub Pages consuming the
  live API via a build-time-configured endpoint.

**Correctness / modernization:**
- Stabilized a legacy C++ graph core on a modern toolchain (C++11 → C++20): fixed a
  heap-corruption bug in edge removal surfaced by libstdc++ assertions, made the
  graph type non-copyable to remove a recursive copy constructor, and got a
  clean-clone build and test suite green in CI.

**Testing:**
- Wrote ~580 assertions across five Catch2 suites plus an 18-check HTTP integration
  test (endpoints, error contract, request limits, readiness, fail-fast startup) and
  frontend unit tests for the API client.

---

## Backable facts & numbers (each is verifiable)

| Claim | Backed by |
| --- | --- |
| C++20; hand-written HTTP/1.1 over raw sockets (Winsock + BSD) | `server.cpp` |
| Graph: 500 airports, 16,379 directed routes | `GET /readyz` returns the counts |
| BFS O(V+E), Dijkstra O(E log V) | `Algorithms.cpp` |
| Betweenness centrality precomputed offline (a full pass is minutes-scale, so not per-request) | `main.cpp` + `results/`; design rationale, not a benchmark |
| Bounded worker pool + bounded queue; HTTP 503 on overload | `ThreadPool.*`, `server.cpp`; `testPool.cpp` proves shedding deterministically |
| 413 on oversized headers (single + segmented); 5s total read deadline | `server.cpp`; `tests/api_test.sh` |
| Config validation with fail-fast on missing airports/routes/centrality; overflow-safe int parsing | `server.cpp`; integration tests |
| Graceful shutdown (SIGINT/SIGTERM → drain queue, join workers) | `server.cpp` |
| ~580 test assertions / 5 Catch2 suites; 18 integration checks; 8 frontend tests | test suites + `api_test.sh` + jest output |
| CI: build/test, ASan/UBSan, Docker build + container route smoke test, frontend build/test | `.github/workflows/ci.yml`, `frontend-ci.yml` |
| Multi-stage, non-root Docker image; deployed on Fly.io behind HTTPS | `Dockerfile`, `fly.toml`, live URL |

---

## Skills / keywords (all genuinely used — safe for a skills section / ATS)

C++20 · multithreading (`std::thread`, `mutex`, `condition_variable`) · thread pools ·
backpressure / load-shedding · BSD/Winsock sockets · HTTP/1.1 · JSON · graph
algorithms (BFS, Dijkstra, Floyd–Warshall, betweenness centrality) · CMake · CTest ·
Catch2 · GitHub Actions (CI) · AddressSanitizer / UBSan · Docker (multi-stage) ·
Fly.io · Linux · Git · React (client)

---

## Interview talking points (turn depth into answers)

- **Why hand-roll the server** instead of a framework: to demonstrate the socket /
  HTTP / concurrency fundamentals end to end; a production service would sit on a
  vetted library — a deliberate, stated tradeoff.
- **Backpressure**: bounded pool caps concurrent compute; bounded queue caps pending
  work; overflow returns 503 instead of growing unbounded.
- **Immutability → lockless reads**: the graph never changes after load, so reads
  need no synchronization; only the queue is guarded.
- **Total read deadline vs idle timeout**: `SO_RCVTIMEO` resets per recv, so a
  dribbling client can outlast it; enforced a whole-request deadline with
  `steady_clock` instead.
- **A real bug you fixed**: `removeEdge` erased an iterator from the wrong list —
  found via libstdc++ assertions aborting at teardown; also deleted a copy
  constructor that recursed into itself.
- **Offline precompute**: betweenness centrality is too expensive to compute per
  request, so it's generated offline and served from a file.
- **Known limitation (say this unprompted)**: socket I/O and route compute share one
  worker pool, so a burst of slow clients can occupy workers — bounded by the read
  deadline and mitigated behind a proxy/edge; the real fix is separating async I/O
  from a dedicated compute pool.

---

## On "distributed systems" — the honest read

This is a **single-node concurrent service, not a distributed system**, so I'd avoid
filing it under "distributed systems" on the résumé. What it *does* demonstrate that
is directly relevant to distributed/infra roles:

- backpressure and load-shedding (HTTP 503 on overload)
- health vs readiness probes (`/healthz` vs `/readyz`) — the Kubernetes/LB pattern
- graceful shutdown / draining
- stateless request handling over shared immutable state
- containerized, non-root deploy behind an HTTPS edge/proxy
- CI/CD (build, test, sanitize, containerize)

To genuinely claim distributed-systems experience you'd want multi-node work —
replication, sharding, consensus/coordination, a message queue, or distributed
caching — which is a different project. (Adding a small horizontally-scaled tier or a
shared cache to this service would be a credible next step if you want to bridge
toward it.)


What's in it:
  - Ready-to-use bullets — your Option A concurrency line, plus
  service/algorithms, lockless reads, CI/sanitizers,
  containerization/deploy, correctness/modernization, and testing.
  Pick 2–3.
  - A backable-facts table — every number tied to what verifies it
  (500 airports / 16,379 routes from /readyz, ~580 assertions across 5
  suites, 18 integration checks, the CI jobs, etc.).
  - Skills/keywords for a skills section — all genuinely used.
  - Interview talking points — the design decisions and the one honest
  limitation (shared I/O+compute pool), which is your strongest "what
  would you improve" answer.
  - The "distributed systems" honesty note — this is a single-node
  service, so I didn't fabricate DS bullets; instead it lists the
  distributed-adjacent things it legitimately shows (backpressure,
  health/readiness, graceful shutdown, containerized deploy behind an
  HTTPS edge, CI/CD) and what a real DS claim would require.

  Two reminders as you write the resume:
  - No perf numbers are in there (per Option A) — nothing about req/s
  or p99, since those aren't measured. If you ever want them, the
  offer stands to run a proper wrk/hey benchmark and cite it with
  conditions.
  - Keep the "backend / C++ / infra / concurrency" framing; treat
  "distributed systems" as adjacent-not-demonstrated unless you build
  the multi-node piece.

  Want me to tailor a tighter 2-bullet version for a specific job
  description, or adjust the wording/length of any of them?
