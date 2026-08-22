#!/usr/bin/env bash
# Endpoint smoke test. CTest passes the server binary as $1 and runs this from the
# repo root so the data paths resolve.
set -u

SERVER="${1:-./build/server}"
TEST_PORT="${AIRPORT_TEST_PORT:-18137}"
BASE="http://127.0.0.1:${TEST_PORT}"
LOG="$(mktemp 2>/dev/null || echo /tmp/api_test_server.log)"
fail=0

PORT="$TEST_PORT" "$SERVER" >"$LOG" 2>&1 &
SERVER_PID=$!
cleanup() {
  kill "$SERVER_PID" 2>/dev/null || true
  wait "$SERVER_PID" 2>/dev/null || true
  rm -f "$LOG"
}
trap cleanup EXIT

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

# Same, but split across TCP segments; curl can't do this, so use a raw socket.
if command -v python3 >/dev/null 2>&1; then
  seg=$(python3 - "$TEST_PORT" <<'PY'
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
  # 413, close, or reset all count as rejected. Only 200 is a failure.
  if [ "$seg" = "200" ]; then
    echo "FAIL [accepted] segmented oversized header"; fail=1
  else
    echo "ok   [$seg] segmented oversized header rejected"
  fi
  check 200 "health after segmented header" "$BASE/healthz"
else
  echo "skip segmented oversized header (python3 not found)"
fi

# A missing data file must abort startup rather than serve empty results.
check_failfast() {
  local desc="$1"; shift
  local port2=$((TEST_PORT + 1))
  local child_log
  child_log="$(mktemp 2>/dev/null || echo "${LOG}.${port2}")"
  env PORT="$port2" "$@" "$SERVER" >"$child_log" 2>&1 &
  local pid=$!
  local up=0
  local exited=0
  local status=0
  for _ in $(seq 1 15); do
    if [ "$(curl -s -o /dev/null -w '%{http_code}' -m 1 "http://127.0.0.1:$port2/readyz" 2>/dev/null)" = "200" ]; then
      up=1; break
    fi
    if ! kill -0 "$pid" 2>/dev/null; then
      wait "$pid" 2>/dev/null
      status=$?
      exited=1
      break
    fi
    sleep 0.2
  done
  if [ "$up" = "1" ]; then
    echo "FAIL [started] $desc"; fail=1
    kill -KILL "$pid" 2>/dev/null
    wait "$pid" 2>/dev/null
  elif [ "$exited" != "1" ]; then
    echo "FAIL [did not exit] $desc"; fail=1
    kill -KILL "$pid" 2>/dev/null
    wait "$pid" 2>/dev/null
  elif [ "$status" = "0" ]; then
    echo "FAIL [exited successfully] $desc"; fail=1
  elif ! grep -q "refusing to start" "$child_log"; then
    echo "FAIL [wrong failure] $desc"; fail=1
    cat "$child_log"
  else
    echo "ok   [failed fast] $desc"
  fi
  rm -f "$child_log"
}

check_failfast "missing edge file fails fast"       AIRPORT_EDGES=/nonexistent-edges.txt
check_failfast "missing centrality file fails fast" AIRPORT_CENTRALITY=/nonexistent-bc.txt

if [ "$fail" = "0" ]; then
  echo "all integration checks passed"
else
  echo "--- server log ---"; cat "$LOG"
fi
exit "$fail"
