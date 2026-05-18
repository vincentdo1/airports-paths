import React, { useState, useRef, useCallback } from 'react';
import GlobeView from './components/GlobeView';
import ControlPanel from './components/ControlPanel';
import InfoCard from './components/InfoCard';
import Tooltip from './components/Tooltip';
import { useArcData } from './hooks/useArcData';
import { useGlobeNavigation } from './hooks/useGlobeNavigation';
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

  useGlobeNavigation(globeEl, selectedAirport);

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

      <InfoCard selectedAirport={selectedAirport} />

      <Tooltip ref={tooltipRef} hoveredAirport={hoveredAirport} />

      <GlobeView
        globeEl={globeEl}
        activeArcs={activeArcs}
        onPointHover={handlePointHover}
        onPointClick={handlePointClick}
      />
    </div>
  );
};

export default AirportGlobe;
