// Bright gold gradient so a computed route stands out against the faint blue
// network arcs. Thicker stroke than the background arcs too (see GlobeView).
const ROUTE_COLOR = ['rgba(255, 215, 0, 0.95)', 'rgba(255, 140, 0, 0.9)'];

// Turn the coordinates the API returns for each airport on the path into the arc
// objects the globe draws — one arc per consecutive leg. Drawing from the API's own
// coordinates (rather than a local table) means every leg of the route renders even
// if the server is serving a larger airport set than the frontend bundles.
export const buildRouteArcs = (coordinates) => {
  if (!coordinates || coordinates.length < 2) return [];
  const arcs = [];
  for (let i = 0; i < coordinates.length - 1; i++) {
    const from = coordinates[i];
    const to   = coordinates[i + 1];
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
