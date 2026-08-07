import React, { useState, useRef, useCallback, useMemo, useEffect } from 'react';
import GlobeView from './components/GlobeView';
import ControlPanel from './components/ControlPanel';
import InfoCard from './components/InfoCard';
import RoutePanel from './components/RoutePanel';
import Tooltip from './components/Tooltip';
import { useArcData } from './hooks/useArcData';
import { useRoute } from './hooks/useRoute';
import { useGlobeNavigation } from './hooks/useGlobeNavigation';
import { buildRouteArcs } from './utils/routeArcs';
import './AirportGlobe.css';

const AirportGlobe = () => {
  const [selectedAirport, setSelectedAirport] = useState('');
  const [hoveredAirport, setHoveredAirport]   = useState(null);

  const globeEl    = useRef(null);
  const tooltipRef = useRef(null);

  const {
    activeArcs,
    showArcs, setShowArcs,
    arcCount,
    sliderValue,
    handleSliderChange,
    handleSliderCommit,
  } = useArcData();

  const {
    source, setSource,
    destination, setDestination,
    mode, setMode,
    status, result, error, latency, coordinates,
    requestRoute,
  } = useRoute();

  useGlobeNavigation(globeEl, selectedAirport);

  // Arc objects for the currently returned route (empty until one is found).
  const routeArcs = useMemo(() => buildRouteArcs(coordinates), [coordinates]);

  // When a new route comes back, swing the camera to its origin so the drawn
  // arc is actually in view.
  useEffect(() => {
    if (status === 'success' && coordinates.length > 0 && globeEl.current) {
      globeEl.current.pointOfView(
        { lat: coordinates[0].lat, lng: coordinates[0].lng, altitude: 2.5 },
        1500
      );
    }
  }, [status, coordinates]);

  // Reposition the tooltip via direct DOM writes — no setState, no re-renders.
  const handleMouseMove = useCallback((e) => {
    if (tooltipRef.current) {
      tooltipRef.current.style.left = `${e.clientX + 14}px`;
      tooltipRef.current.style.top  = `${e.clientY - 36}px`;
    }
  }, []);

  // Stable callbacks so GlobeView (React.memo) never re-renders from these.
  const handlePointHover = useCallback((point) => setHoveredAirport(point), []);
  const handlePointClick = useCallback((point) => setSelectedAirport(point.id), []);

  return (
    <div className="globe-container" onMouseMove={handleMouseMove}>
      <ControlPanel
        selectedAirport={selectedAirport}
        onAirportChange={setSelectedAirport}
        showArcs={showArcs}
        onToggleArcs={() => setShowArcs(v => !v)}
        arcCount={arcCount}
        sliderValue={sliderValue}
        onSliderChange={handleSliderChange}
        onSliderCommit={handleSliderCommit}
      />

      <RoutePanel
        source={source}
        destination={destination}
        mode={mode}
        onSourceChange={setSource}
        onDestinationChange={setDestination}
        onModeChange={setMode}
        status={status}
        result={result}
        error={error}
        latency={latency}
        onFindRoute={requestRoute}
      />

      <InfoCard selectedAirport={selectedAirport} />

      <Tooltip ref={tooltipRef} hoveredAirport={hoveredAirport} />

      <GlobeView
        globeEl={globeEl}
        activeArcs={activeArcs}
        routeArcs={routeArcs}
        onPointHover={handlePointHover}
        onPointClick={handlePointClick}
      />
    </div>
  );
};

export default AirportGlobe;
