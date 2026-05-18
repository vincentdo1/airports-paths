import React from 'react';
import nodesData from '../../data/nodes_data.json';
import { TOP_AIRPORTS, AIRPORT_NAMES } from '../../constants/airports';
import './ControlPanel.css';

const GitHubIcon = () => (
  <svg width="16" height="16" viewBox="0 0 24 24" fill="currentColor">
    <path d="M12 0C5.37 0 0 5.37 0 12c0 5.31 3.435 9.795 8.205 11.385.6.105.825-.255.825-.57 0-.285-.015-1.23-.015-2.235-3.015.555-3.795-.735-4.035-1.41-.135-.345-.72-1.41-1.23-1.695-.42-.225-1.02-.78-.015-.795.945-.015 1.62.87 1.845 1.23 1.08 1.815 2.805 1.305 3.495.99.105-.78.42-1.305.765-1.605-2.67-.3-5.46-1.335-5.46-5.925 0-1.305.465-2.385 1.23-3.225-.12-.3-.54-1.53.12-3.18 0 0 1.005-.315 3.3 1.23.96-.27 1.98-.405 3-.405s2.04.135 3 .405c2.295-1.56 3.3-1.23 3.3-1.23.66 1.65.24 2.88.12 3.18.765.84 1.23 1.905 1.23 3.225 0 4.605-2.805 5.625-5.475 5.925.435.375.81 1.095.81 2.22 0 1.605-.015 2.895-.015 3.3 0 .315.225.69.825.57A12.02 12.02 0 0 0 24 12c0-6.63-5.37-12-12-12z" />
  </svg>
);

const ControlPanel = ({
  selectedAirport,
  onAirportChange,
  showArcs,
  onToggleArcs,
  sliderValue,
  arcCount,
  onSliderChange,
  onSliderCommit,
}) => (
  <div className="control-panel">
    <div className="panel-header">
      <span className="panel-logo">✈</span>
      <h1 className="panel-title">Airport Network</h1>
      <p className="panel-subtitle">Global hub analysis via Betweenness Centrality</p>
    </div>

    <div className="panel-divider" />

    <div className="panel-section">
      <label className="section-label">Navigate To Airport</label>
      <div className="select-wrapper">
        <select
          className="airport-select"
          value={selectedAirport}
          onChange={(e) => onAirportChange(e.target.value)}
        >
          <option value="">— Select an airport —</option>
          <optgroup label="Top 10 Global Hubs">
            {TOP_AIRPORTS.map(id => (
              <option key={id} value={id}>{id} — {AIRPORT_NAMES[id]}</option>
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
          onClick={onToggleArcs}
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
            onChange={onSliderChange}
            onMouseUp={onSliderCommit}
            onTouchEnd={onSliderCommit}
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
        <span className="stat-number">{TOP_AIRPORTS.length}</span>
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
);

export default ControlPanel;
