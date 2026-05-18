import React, { useEffect, useState, useRef, useCallback, useMemo } from 'react';
import debounce from 'lodash.debounce';
import Globe from 'react-globe.gl';
import nodesData from '../resources/nodes_data.json';
import arcsData from '../resources/arcs_data.json';
import './global.css';

const TOP_AIRPORTS = ['PEK', 'IST', 'SVO', 'DEL', 'URC', 'ICN', 'DXB', 'KUL', 'CCU', 'YYZ'];

const AIRPORT_NAMES = {
  PEK: 'Beijing Capital Intl',
  IST: 'Istanbul Airport',
  SVO: 'Moscow Sheremetyevo',
  DEL: 'Indira Gandhi Intl',
  URC: 'Ürümqi Diwopu Intl',
  ICN: 'Seoul Incheon Intl',
  DXB: 'Dubai International',
  KUL: 'Kuala Lumpur Intl',
  CCU: 'Kolkata Netaji Subhas',
  YYZ: 'Toronto Pearson Intl',
};

// ─── Module-level constants ────────────────────────────────────────────────
// Computed once at load time. Stable object references prevent React.memo
// from seeing "changed" props on every parent re-render.

const enrichedAirports = nodesData.map(airport => ({
  ...airport,
  isTop: TOP_AIRPORTS.includes(airport.id),
  color: TOP_AIRPORTS.includes(airport.id) ? '#FFD700' : 'rgba(100, 190, 255, 0.75)',
  size: TOP_AIRPORTS.includes(airport.id) ? 0.5 : 0.15,
}));

const topAirports = enrichedAirports.filter(a => a.isTop);

// Stable function refs — inline arrow functions in JSX props are new objects
// every render and break React.memo's shallow comparison.
const LABEL_COLOR = () => 'rgba(255, 220, 50, 0.95)';
const POINT_LABEL = () => '';
const EMPTY_ARR = [];

// Pre-bake random per-arc values into the data so Globe reads them as string
// accessors (stable) rather than calling a new Math.random() function on
// every render cycle.
const sampleArcs = (data, n) => {
  const shuffled = [...data].sort(() => 0.5 - Math.random());
  return shuffled.slice(0, n).map(arc => ({
    ...arc,
    arcColor: ['rgba(0, 180, 255, 0.65)', 'rgba(0, 60, 180, 0.08)'],
    dashLength: Math.random() * 0.4 + 0.1,
    dashGap:    Math.random() * 0.4 + 0.1,
    animateTime: Math.random() * 4000 + 1000,
  }));
};

// ─── Memoized Globe wrapper ────────────────────────────────────────────────
// Isolates the Three.js scene from the React subtree above it.
// Only re-renders when arc data changes or point callbacks fire — NOT on
// mouse moves, tooltip state, or info-card visibility changes.
const GlobeView = React.memo(({ globeEl, activeArcs, onPointHover, onPointClick }) => (
  <Globe
    ref={globeEl}
    globeImageUrl="//unpkg.com/three-globe/example/img/earth-night.jpg"
    backgroundImageUrl="//unpkg.com/three-globe/example/img/night-sky.png"
    atmosphereColor="rgba(40, 100, 255, 0.35)"
    atmosphereAltitude={0.2}
    pointsData={enrichedAirports}
    pointColor="color"
    pointAltitude="size"
    pointRadius={0.4}
    pointLabel={POINT_LABEL}
    onPointHover={onPointHover}
    onPointClick={onPointClick}
    labelsData={topAirports}
    labelText="id"
    labelLat="lat"
    labelLng="lng"
    labelAltitude={0.025}
    labelSize={1.0}
    labelColor={LABEL_COLOR}
    labelResolution={2}
    labelIncludeDot={false}
    arcsData={activeArcs}
    arcColor="arcColor"
    arcDashLength="dashLength"
    arcDashGap="dashGap"
    arcDashAnimateTime="animateTime"
    arcStroke={0.5}
    arcTransitionDuration={0}
  />
));
GlobeView.displayName = 'GlobeView';

// ─── GitHubIcon ────────────────────────────────────────────────────────────
const GitHubIcon = () => (
  <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor">
    <path d="M12 0C5.37 0 0 5.37 0 12c0 5.31 3.435 9.795 8.205 11.385.6.105.825-.255.825-.57 0-.285-.015-1.23-.015-2.235-3.015.555-3.795-.735-4.035-1.41-.135-.345-.72-1.41-1.23-1.695-.42-.225-1.02-.78-.015-.795.945-.015 1.62.87 1.845 1.23 1.08 1.815 2.805 1.305 3.495.99.105-.78.42-1.305.765-1.605-2.67-.3-5.46-1.335-5.46-5.925 0-1.305.465-2.385 1.23-3.225-.12-.3-.54-1.53.12-3.18 0 0 1.005-.315 3.3 1.23.96-.27 1.98-.405 3-.405s2.04.135 3 .405c2.295-1.56 3.3-1.23 3.3-1.23.66 1.65.24 2.88.12 3.18.765.84 1.23 1.905 1.23 3.225 0 4.605-2.805 5.625-5.475 5.925.435.375.81 1.095.81 2.22 0 1.605-.015 2.895-.015 3.3 0 .315.225.69.825.57A12.02 12.02 0 0 0 24 12c0-6.63-5.37-12-12-12z" />
  </svg>
);

// ─── Main component ────────────────────────────────────────────────────────
const MyGlobe = () => {
  const [selectedAirport, setSelectedAirport] = useState('');
  const [hoveredAirport, setHoveredAirport] = useState(null);
  const [displayArcs, setDisplayArcs] = useState(() => sampleArcs(arcsData, 500));
  const [showArcs, setShowArcs] = useState(true);
  // arcCount drives the actual re-sample; sliderValue drives the visible number
  // on the slider — only committed to arcCount on mouseup so the expensive
  // shuffle+Globe reinit doesn't fire on every drag tick.
  const [arcCount, setArcCount] = useState(500);
  const [sliderValue, setSliderValue] = useState(500);

  const globeEl = useRef(null);
  const tooltipRef = useRef(null); // positioned via direct DOM writes, not state

  // Re-sample arcs only when arcCount commits (slider mouseup / touch end)
  useEffect(() => {
    setDisplayArcs(sampleArcs(arcsData, arcCount));
  }, [arcCount]);

  // Fly the camera to the selected airport
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

  // Update the tooltip position via direct DOM writes instead of setState.
  // This keeps ~60 re-renders/sec from mouse movement out of React entirely.
  const handleMouseMove = useCallback((e) => {
    if (tooltipRef.current) {
      tooltipRef.current.style.left = `${e.clientX + 14}px`;
      tooltipRef.current.style.top  = `${e.clientY - 36}px`;
    }
  }, []);

  // Stable callbacks — useCallback with [] deps so React.memo on GlobeView
  // sees the same reference across parent re-renders.
  const handlePointHover = useCallback((point) => setHoveredAirport(point), []);
  const handlePointClick = useCallback((point) => setSelectedAirport(point.id), []);

  // Slider: update display immediately, commit (re-sample) only on release
  const handleSliderChange  = (e) => setSliderValue(Number(e.target.value));
  const handleSliderCommit  = (e) => {
    const val = Number(e.target.value);
    setSliderValue(val);
    setArcCount(val);
  };

  // Avoid a new [] literal on every render when arcs are hidden
  const activeArcs = useMemo(
    () => (showArcs ? displayArcs : EMPTY_ARR),
    [showArcs, displayArcs]
  );

  const selectedData = enrichedAirports.find(a => a.id === selectedAirport);
  const rank = TOP_AIRPORTS.indexOf(selectedAirport);

  return (
    <div className="globe-container" onMouseMove={handleMouseMove}>

      {/* ── Control Panel ── */}
      <div className="control-panel">
        <div className="panel-header">
          <span className="panel-logo">✈</span>
          <h1 className="panel-title">Airport Network</h1>
          <p className="panel-subtitle">
            Global hub analysis via Betweenness Centrality
          </p>
        </div>

        <div className="panel-divider" />

        <div className="panel-section">
          <label className="section-label">Navigate To Airport</label>
          <div className="select-wrapper">
            <select
              className="airport-select"
              value={selectedAirport}
              onChange={(e) => setSelectedAirport(e.target.value)}
            >
              <option value="">— Select an airport —</option>
              <optgroup label="Top 10 Global Hubs">
                {TOP_AIRPORTS.map(id => (
                  <option key={id} value={id}>
                    {id} — {AIRPORT_NAMES[id]}
                  </option>
                ))}
              </optgroup>
              <optgroup label="All Airports">
                {nodesData
                  .filter(a => !TOP_AIRPORTS.includes(a.id))
                  .map(a => <option key={a.id} value={a.id}>{a.id}</option>)}
              </optgroup>
            </select>
            <span className="select-arrow">▾</span>
          </div>
          <p className="section-hint">Or click any airport on the globe</p>
        </div>

        <div className="panel-divider" />

        <div className="panel-section">
          <div className="section-row">
            <label className="section-label">Flight Routes</label>
            <button
              className={`toggle-btn ${showArcs ? 'on' : 'off'}`}
              onClick={() => setShowArcs(v => !v)}
            >
              {showArcs ? 'ON' : 'OFF'}
            </button>
          </div>
          {showArcs && (
            <div className="slider-group">
              <div className="slider-info">
                <span className="slider-label">Showing</span>
                <span className="slider-value">{sliderValue.toLocaleString()} routes</span>
              </div>
              <input
                type="range"
                min={100}
                max={3000}
                step={100}
                value={sliderValue}
                onChange={handleSliderChange}
                onMouseUp={handleSliderCommit}
                onTouchEnd={handleSliderCommit}
                className="arc-slider"
              />
              <div className="slider-range">
                <span>100</span>
                <span>3,000</span>
              </div>
            </div>
          )}
        </div>

        <div className="panel-divider" />

        <div className="panel-section">
          <label className="section-label">Legend</label>
          <div className="legend-items">
            <div className="legend-item">
              <div className="legend-dot gold-dot" />
              <span>Top 10 Global Hubs</span>
            </div>
            <div className="legend-item">
              <div className="legend-dot blue-dot" />
              <span>Regional Airports</span>
            </div>
            <div className="legend-item">
              <div className="legend-arc" />
              <span>Flight Routes</span>
            </div>
          </div>
        </div>

        <div className="panel-divider" />

        <div className="panel-stats">
          <div className="stat-item">
            <span className="stat-number">{nodesData.length}</span>
            <span className="stat-label">Airports</span>
          </div>
          <div className="stat-divider" />
          <div className="stat-item">
            <span className="stat-number">{arcCount.toLocaleString()}</span>
            <span className="stat-label">Routes</span>
          </div>
          <div className="stat-divider" />
          <div className="stat-item">
            <span className="stat-number">10</span>
            <span className="stat-label">Top Hubs</span>
          </div>
        </div>

        <a
          href="https://github.com/vincentdo1/airports-paths"
          target="_blank"
          rel="noopener noreferrer"
          className="github-link"
        >
          <GitHubIcon />
          View on GitHub
        </a>
      </div>

      {/* ── Selected Airport Info Card ── */}
      {selectedAirport && selectedData && (
        <div className="info-card" key={selectedAirport}>
          <div className="info-card-header">
            <span className="info-iata">{selectedAirport}</span>
            {rank >= 0 && (
              <span className="info-rank-badge">#{rank + 1} Global Hub</span>
            )}
          </div>
          {AIRPORT_NAMES[selectedAirport] && (
            <p className="info-name">{AIRPORT_NAMES[selectedAirport]}</p>
          )}
          <div className="info-coords">
            <div className="coord-item">
              <span className="coord-label">LAT</span>
              <span className="coord-value">{selectedData.lat.toFixed(3)}°</span>
            </div>
            <div className="coord-item">
              <span className="coord-label">LNG</span>
              <span className="coord-value">{selectedData.lng.toFixed(3)}°</span>
            </div>
          </div>
          {rank >= 0 && (
            <p className="info-bc-note">
              High betweenness centrality — a critical routing hub in the global flight network.
            </p>
          )}
        </div>
      )}

      {/* ── Hover Tooltip ── */}
      {/* Always mounted so it can be positioned via the DOM ref without state.
          Visibility is controlled by the CSS class, not conditional rendering. */}
      <div
        ref={tooltipRef}
        className={`hover-tooltip ${hoveredAirport ? 'tooltip-visible' : ''}`}
      >
        {hoveredAirport && (
          <>
            <span className="tooltip-id">{hoveredAirport.id}</span>
            {TOP_AIRPORTS.includes(hoveredAirport.id) && (
              <span className="tooltip-hub">Top Hub</span>
            )}
          </>
        )}
      </div>

      {/* ── Globe ── */}
      <GlobeView
        globeEl={globeEl}
        activeArcs={activeArcs}
        onPointHover={handlePointHover}
        onPointClick={handlePointClick}
      />
    </div>
  );
};

export default MyGlobe;
