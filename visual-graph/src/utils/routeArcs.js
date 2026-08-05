import { enrichedAirports } from './enrichAirports';

// Bright gold gradient so a computed route stands out against the faint blue
// network arcs. Thicker stroke than the background arcs too (see GlobeView).
const ROUTE_COLOR = ['rgba(255, 215, 0, 0.95)', 'rgba(255, 140, 0, 0.9)'];

// Look up airport coordinates by IATA id once.
const coordsById = enrichedAirports.reduce((acc, a) => {
  acc[a.id] = { lat: a.lat, lng: a.lng };
  return acc;
}, {});

// Turn an ordered path of airport codes (["PEK", "SEA", "NRT"]) into the arc
// objects the globe draws — one arc per consecutive leg. Legs whose endpoints
// aren't in our coordinate table are skipped rather than dropped as a whole.
export const buildRouteArcs = (path) => {
  if (!path || path.length < 2) return [];
  const arcs = [];
  for (let i = 0; i < path.length - 1; i++) {
    const from = coordsById[path[i]];
    const to   = coordsById[path[i + 1]];
    if (!from || !to) continue;
    arcs.push({
      startLat:    from.lat,
      startLng:    from.lng,
      endLat:      to.lat,
      endLng:      to.lng,
      arcColor:    ROUTE_COLOR,
      dashLength:  0.4,
      dashGap:     0.15,
      animateTime: 1500,
      stroke:      1.3,
    });
  }
  return arcs;
};
