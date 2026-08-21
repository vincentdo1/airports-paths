# Data

Derived from the [OpenFlights](https://openflights.org/data.html) airport and route
database, which is published under the Open Database License (ODbL). See the
OpenFlights site for the current terms and attribution requirements.

- `airports.csv`, `routes.csv` — raw OpenFlights extracts
- `cleaner.py` — turns those into the whitespace-separated node/edge files
- `nodes500.txt` / `edges500.txt`, `nodes1000.txt` / `edges1000.txt` — the 500 and
  1000 busiest airports and the routes among them
- `nodes.txt` / `edges.txt` — small sample used by the unit tests

Node files are `CODE latitude longitude`, edge files are `EDGEID source destination`,
one per line. Precomputed betweenness centrality lives in `results/`.
