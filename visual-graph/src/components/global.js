import React, { useEffect, useState, useRef, useCallback } from 'react';
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

const GitHubIcon = () => (
  <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor">
    <path d="M12 0C5.37 0 0 5.37 0 12c0 5.31 3.435 9.795 8.205 11.385.6.105.825-.255.825-.57 0-.285-.015-1.23-.015-2.235-3.015.555-3.795-.735-4.035-1.41-.135-.345-.72-1.41-1.23-1.695-.42-.225-1.02-.78-.015-.795.945-.015 1.62.87 1.845 1.23 1.08 1.815 2.805 1.305 3.495.99.105-.78.42-1.305.765-1.605-2.67-.3-5.46-1.335-5.46-5.925 0-1.305.465-2.385 1.23-3.225-.12-.3-.54-1.53.12-3.18 0 0 1.005-.315 3.3 1.23.96-.27 1.98-.405 3-.405s2.04.135 3 .405c2.295-1.56 3.3-1.23 3.3-1.23.66 1.65.24 2.88.12 3.18.765.84 1.23 1.905 1.23 3.225 0 4.605-2.805 5.625-5.475 5.925.435.375.81 1.095.81 2.22 0 1.605-.015 2.895-.015 3.3 0 .315.225.69.825.57A12.02 12.02 0 0 0 24 12c0-6.63-5.37-12-12-12z" />
  </svg>
);

const MyGlobe = () => {
  const [airports, setAirports] = useState([]);
  const [selectedAirport, setSelectedAirport] = useState('');
  const [hoveredAirport, setHoveredAirport] = useState(null);
  const [displayArcs, setDisplayArcs] = useState([]);
  const [showArcs, setShowArcs] = useState(true);
  const [arcCount, setArcCount] = useState(2000);
  const [mousePos, setMousePos] = useState({ x: 0, y: 0 });
  const globeEl = useRef(null);

  useEffect(() => {
    const enriched = nodesData.map(airport => ({
      ...airport,
      isTop: TOP_AIRPORTS.includes(airport.id),
      color: TOP_AIRPORTS.includes(airport.id)
        ? '#FFD700'
        : 'rgba(100, 190, 255, 0.75)',
      size: TOP_AIRPORTS.includes(airport.id) ? 0.5 : 0.15,
    }));
    setAirports(enriched);
  }, []);

  useEffect(() => {
    const sample = (data, n) => {
      const shuffled = [...data].sort(() => 0.5 - Math.random());
      return shuffled.slice(0, n);
    };
    setDisplayArcs(sample(arcsData, arcCount));
  }, [arcCount]);

  const airportsRef = useRef(airports);
  useEffect(() => { airportsRef.current = airports; }, [airports]);

  // eslint-disable-next-line react-hooks/exhaustive-deps
  const goToAirport = useCallback(
    debounce((airportID) => {
      const airport = airportsRef.current.find(a => a.id === airportID);
      if (airport && globeEl.current) {
        globeEl.current.pointOfView(
          { lat: airport.lat, lng: airport.lng, altitude: 2.5 },
          2000
        );
      }
    }, 200),
    []
  );

  useEffect(() => {
    if (selectedAirport) goToAirport(selectedAirport);
  }, [selectedAirport, goToAirport]);

  const handleMouseMove = useCallback((e) => {
    setMousePos({ x: e.clientX, y: e.clientY });
  }, []);

  const handlePointClick = useCallback((point) => {
    setSelectedAirport(point.id);
  }, []);

  const selectedData = airports.find(a => a.id === selectedAirport);
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

        {/* Airport selector */}
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
                  .map(a => (
                    <option key={a.id} value={a.id}>{a.id}</option>
                  ))}
              </optgroup>
            </select>
            <span className="select-arrow">▾</span>
          </div>
          <p className="section-hint">Or click any airport on the globe</p>
        </div>

        <div className="panel-divider" />

        {/* Arc toggle + slider */}
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
                <span className="slider-value">{arcCount.toLocaleString()} routes</span>
              </div>
              <input
                type="range"
                min={100}
                max={5000}
                step={100}
                value={arcCount}
                onChange={(e) => setArcCount(Number(e.target.value))}
                className="arc-slider"
              />
              <div className="slider-range">
                <span>100</span>
                <span>5,000</span>
              </div>
            </div>
          )}
        </div>

        <div className="panel-divider" />

        {/* Legend */}
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

        {/* Stats */}
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
      {hoveredAirport && (
        <div
          className="hover-tooltip"
          style={{ left: mousePos.x + 14, top: mousePos.y - 36 }}
        >
          <span className="tooltip-id">{hoveredAirport.id}</span>
          {TOP_AIRPORTS.includes(hoveredAirport.id) && (
            <span className="tooltip-hub">Top Hub</span>
          )}
        </div>
      )}

      {/* ── Globe ── */}
      <Globe
        ref={globeEl}
        globeImageUrl="//unpkg.com/three-globe/example/img/earth-night.jpg"
        backgroundImageUrl="//unpkg.com/three-globe/example/img/night-sky.png"
        atmosphereColor="rgba(40, 100, 255, 0.35)"
        atmosphereAltitude={0.2}
        pointsData={airports}
        pointColor="color"
        pointAltitude="size"
        pointRadius={0.4}
        pointLabel={() => ''}
        onPointHover={setHoveredAirport}
        onPointClick={handlePointClick}
        labelsData={airports.filter(a => a.isTop)}
        labelText="id"
        labelLat="lat"
        labelLng="lng"
        labelAltitude={0.025}
        labelSize={1.0}
        labelColor={() => 'rgba(255, 220, 50, 0.95)'}
        labelResolution={2}
        labelIncludeDot={false}
        arcsData={showArcs ? displayArcs : []}
        arcColor={() => ['rgba(0, 180, 255, 0.65)', 'rgba(0, 60, 180, 0.08)']}
        arcDashLength={() => Math.random() * 0.4 + 0.1}
        arcDashGap={() => Math.random() * 0.4 + 0.1}
        arcDashAnimateTime={() => Math.random() * 4000 + 1000}
        arcStroke={0.5}
      />
    </div>
  );
};

export default MyGlobe;
