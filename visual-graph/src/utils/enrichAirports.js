import nodesData from '../data/nodes_data.json';
import { TOP_AIRPORTS } from '../constants/airports';

export const enrichedAirports = nodesData.map(airport => ({
  ...airport,
  isTop: TOP_AIRPORTS.includes(airport.id),
  color: TOP_AIRPORTS.includes(airport.id) ? '#FFD700' : 'rgba(100, 190, 255, 0.75)',
  size:  TOP_AIRPORTS.includes(airport.id) ? 0.5 : 0.15,
}));

export const topAirports = enrichedAirports.filter(a => a.isTop);
