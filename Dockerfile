#   docker build -t airport-routing .
#   docker run --rm -p 8080:8080 airport-routing

FROM debian:bookworm-slim AS build
RUN apt-get update && apt-get install -y --no-install-recommends \
      g++ cmake make \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY . .
RUN cmake --preset release \
    && cmake --build --preset release --target server

# runtime: just the binary, its data, and libstdc++
FROM debian:bookworm-slim AS runtime
RUN apt-get update && apt-get install -y --no-install-recommends \
      libstdc++6 \
    && rm -rf /var/lib/apt/lists/* \
    && useradd --system --no-create-home app
WORKDIR /app
COPY --from=build /src/build/release/server /app/server
COPY data/ /app/data/
COPY results/ /app/results/
USER app

# hosts inject PORT; dataset paths are relative to WORKDIR
ENV PORT=8080 \
    AIRPORT_NODES=data/nodes500.txt \
    AIRPORT_EDGES=data/edges500.txt \
    AIRPORT_CENTRALITY=results/Sorted500BC.txt
EXPOSE 8080
CMD ["/app/server"]
