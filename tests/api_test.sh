#!/usr/bin/env bash
# Integration smoke test for the HTTP service. Starts the server on a test port,
# waits for it to become ready, exercises every endpoint (happy paths, error cases,
# and the request-hardening cases), and checks the HTTP status codes. Invoked by
# CTest with the server binary path as $1, run from the repository root so the data
# files resolve. Exits non-zero if any check fails.
set -u

SERVER="${1:-./build/server}"
PORT="${AIRPORT_TEST_PORT:-18137}"
BASE="http://127.0.0.1:${PORT}"
LOG="$(mktemp 2>/dev/null || echo /tmp/api_test_server.log)"
fail=0

AIRPORT_PORT="$PORT" "$SERVER" >"$LOG" 2>&1 &
SERVER_PID=$!
trap 'kill "$SERVER_PID" 2>/dev/null' EXIT

# Wait up to ~10s for readiness.
ready=0
for _ in $(seq 1 50); do
  if [ "$(curl -s -o /dev/null -w '%{http_code}' -m 2 "$BASE/readyz" 2>/dev/null || true)" = "200" ]; then
    ready=1; break
  fi
  sleep 0.2
done
if [ "$ready" != "1" ]; then
  echo "server did not become ready"; cat "$LOG"; exit 1
fi

check() {
  # check <expected-status> <description> <curl args...>
  local expected="$1"; shift
  local desc="$1"; shift
  local code
  code=$(curl -s -o /dev/null -w '%{http_code}' -m 5 "$@" 2>/dev/null || true)
  if [ "$code" = "$expected" ]; then
    echo "ok   [$code] $desc"
  else
    echo "FAIL [got $code, wanted $expected] $desc"; fail=1
  fi
}

check 200 "healthz"               "$BASE/healthz"
check 200 "readyz"                "$BASE/readyz"
check 200 "airport lookup"        "$BASE/api/v1/airports/PEK"
check 404 "unknown airport"       "$BASE/api/v1/airports/ZZZ"
check 200 "route by hops"         "$BASE/api/v1/routes?source=PEK&destination=JFK&mode=hops"
check 200 "route by distance"     "$BASE/api/v1/routes?source=PEK&destination=JFK&mode=distance"
check 400 "missing destination"   "$BASE/api/v1/routes?source=PEK"
check 400 "unsupported mode"      "$BASE/api/v1/routes?source=PEK&destination=JFK&mode=teleport"
check 404 "route unknown airport" "$BASE/api/v1/routes?source=PEK&destination=ZZZ"
check 200 "central airports"      "$BASE/api/v1/network/central-airports?limit=5"
check 400 "central bad limit"     "$BASE/api/v1/network/central-airports?limit=abc"
check 400 "central overflow limit" "$BASE/api/v1/network/central-airports?limit=4294967296"
check 404 "unknown endpoint"      "$BASE/nope"
check 405 "unsupported method"    -X POST "$BASE/healthz"

# Oversized request header must be rejected with 413.
big=$(head -c 20000 </dev/zero | tr '\0' 'a')
check 413 "oversized header"      -H "X-Big: $big" "$BASE/healthz"

# Oversized headers split across TCP segments must also be rejected. A single curl
# can't reproduce this, so drive it with a raw socket if python3 is available.
if command -v python3 >/dev/null 2>&1; then
  seg=$(python3 - "$PORT" <<'PY'
import socket, sys, time
port = int(sys.argv[1])
code = "000"
try:
    s = socket.create_connection(("127.0.0.1", port), timeout=5)
    try:
        s.sendall(b"GET /healthz HTTP/1.1\r\nX-Big: " + b"a" * 15000)
        time.sleep(0.1)
        s.sendall(b"a" * 2004 + b"\r\n\r\n")
    except OSError:
        pass  # the server may have rejected and closed before we finished sending
    resp = s.recv(256)
    code = resp.split(b" ")[1].decode() if resp.startswith(b"HTTP/") else "closed"
    s.close()
except OSError:
    code = "reset"
print(code)
PY
)
  # The header exceeds the cap, so it must not be accepted (anything but 200: a 413,
  # or a close/reset once the server has seen enough, all count as rejected).
  if [ "$seg" = "200" ]; then
    echo "FAIL [accepted] segmented oversized header"; fail=1
  else
    echo "ok   [$seg] segmented oversized header rejected"
  fi
else
  echo "skip segmented oversized header (python3 not found)"
fi

# Startup must fail fast (not serve) when a required data file is missing: start the
# server with a bad config on another port and confirm it never becomes ready.
check_failfast() {
  local desc="$1"; shift
  local port2=$((PORT + 1))
  env AIRPORT_PORT="$port2" "$@" "$SERVER" >/dev/null 2>&1 &
  local pid=$!
  local up=0
  for _ in $(seq 1 15); do
    if [ "$(curl -s -o /dev/null -w '%{http_code}' -m 1 "http://127.0.0.1:$port2/readyz" 2>/dev/null)" = "200" ]; then
      up=1; break
    fi
    kill -0 "$pid" 2>/dev/null || break   # process already exited => it failed fast
    sleep 0.2
  done
  if [ "$up" = "1" ]; then
    echo "FAIL [started] $desc"; fail=1; kill "$pid" 2>/dev/null
  else
    echo "ok   [failed fast] $desc"
  fi
  wait "$pid" 2>/dev/null
}

check_failfast "missing edge file fails fast"       AIRPORT_EDGES=/nonexistent-edges.txt
check_failfast "missing centrality file fails fast" AIRPORT_CENTRALITY=/nonexistent-bc.txt

if [ "$fail" = "0" ]; then
  echo "all integration checks passed"
else
  echo "--- server log ---"; cat "$LOG"
fi
exit "$fail"
