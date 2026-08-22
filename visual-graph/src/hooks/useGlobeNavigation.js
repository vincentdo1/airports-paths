import { useEffect } from 'react';
import { enrichedAirports } from '../utils/enrichAirports';

export const useGlobeNavigation = (globeEl, selectedAirport) => {
  useEffect(() => {
    const airport = enrichedAirports.find(({ id }) => id === selectedAirport);
    if (airport && globeEl.current) {
      globeEl.current.pointOfView(
        { lat: airport.lat, lng: airport.lng, altitude: 2.5 },
        2000
      );
    }
  }, [globeEl, selectedAirport]);
};
