# Data

The airport and route files are derived from the
[OpenFlights](https://openflights.org/data) database, published under the Open
Database License (ODbL). OpenFlights stopped receiving route updates in June 2014
and describes that snapshot as historical rather than current schedule data. See
its site for the current terms and attribution requirements.

The nominal 500- and 1000-airport subsets use GetToCenter's
[2017 passenger-traffic ranking](https://gettocenter.com/airports/top-100-airports-in-world/1000).
Not every ranked code has a matching OpenFlights record, so the filenames describe
ranking cutoffs rather than guaranteed record counts.

- `airports.csv`, `routes.csv` — raw OpenFlights extracts
- `cleaner.py` — prepares full-network files and the nominal 1000-airport subset
- `nodes500.txt` / `edges500.txt`, `nodes1000.txt` / `edges1000.txt` — ranked subsets
- `nodes.txt` / `edges.txt` — small sample used by the unit tests

Node files are `CODE latitude longitude`, edge files are `EDGEID source destination`,
one per line. Some subset edge rows reference airports absent from the paired node
file; `AdjList` skips a route when either endpoint did not load. Precomputed
betweenness centrality snapshots live in `results/`.
