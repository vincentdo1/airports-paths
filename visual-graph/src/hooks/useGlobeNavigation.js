import { useEffect, useCallback } from 'react';
import debounce from 'lodash.debounce';
import { enrichedAirports } from '../utils/enrichAirports';

export const useGlobeNavigation = (globeEl, selectedAirport) => {
  // eslint-disable-next-line react-hooks/exhaustive-deps
  const goToAirport = useCallback(
    debounce((id) => {
      const apt = enrichedAirports.find(a => a.id === id);
      if (apt && globeEl.current) {
        globeEl.current.pointOfView(
          { lat: apt.lat, lng: apt.lng, altitude: 2.5 },
          2000
        );
      }
    }, 200),
    []
  );

  useEffect(() => {
    if (selectedAirport) goToAirport(selectedAirport);
  }, [selectedAirport, goToAirport]);
};
