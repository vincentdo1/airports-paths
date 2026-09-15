const ROUTE_COLOR = ['rgba(255, 215, 0, 0.95)', 'rgba(255, 140, 0, 0.9)'];

// One arc per leg, built from the coordinates the API returns rather than a local
// table, so routes still draw when the server has more airports than we bundle.
export const buildRouteArcs = (coordinates) => {
  if (!coordinates || coordinates.length < 2) return [];
  const arcs = [];
  for (let i = 0; i < coordinates.length - 1; i++) {
    const from = coordinates[i];
    const to = coordinates[i + 1];
    arcs.push({
      startLat: from.lat,
      startLng: from.lng,
      endLat: to.lat,
      endLng: to.lng,
      arcColor: ROUTE_COLOR,
      dashLength: 0.4,
      dashGap: 0.15,
      animateTime: 1500,
      stroke: 1.3,
    });
  }
  return arcs;
};
