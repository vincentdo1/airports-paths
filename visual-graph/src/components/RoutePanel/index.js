import React from 'react';
import nodesData from '../../data/nodes_data.json';
import './RoutePanel.css';

const ERROR_TITLES = {
  MISSING_PARAMETER: 'Missing input',
  UNKNOWN_AIRPORT: 'Unknown airport',
  NO_ROUTE: 'No route found',
  UNSUPPORTED_MODE: 'Unsupported mode',
  OVERLOADED: 'Server busy',
  SERVER_UNAVAILABLE: 'Service offline',
  TIMEOUT: 'Timed out',
};

const RoutePanel = ({
  source,
  destination,
  mode,
  onSourceChange,
  onDestinationChange,
  onModeChange,
  status,
  result,
  error,
  latency,
  onFindRoute,
}) => {
  const handleSubmit = (event) => {
    event.preventDefault();
    if (status !== 'loading') onFindRoute();
  };

  return (
    <form className="route-panel" aria-label="Route search" onSubmit={handleSubmit}>
      <div className="route-header">
        <span className="route-logo">⇄</span>
        <h2 className="route-title">Find a Route</h2>
        <p className="route-subtitle">Shortest path between two airports, computed by the live API</p>
      </div>

      <div className="route-fields">
        <div className="route-field">
          <label className="section-label" htmlFor="route-source">From</label>
          <input
            id="route-source"
            className="route-input"
            list="airport-codes"
            value={source}
            onChange={(e) => onSourceChange(e.target.value.toUpperCase())}
            placeholder="e.g. ORD"
            maxLength={3}
          />
        </div>
        <span className="route-arrow-icon">→</span>
        <div className="route-field">
          <label className="section-label" htmlFor="route-destination">To</label>
          <input
            id="route-destination"
            className="route-input"
            list="airport-codes"
            value={destination}
            onChange={(e) => onDestinationChange(e.target.value.toUpperCase())}
            placeholder="e.g. NRT"
            maxLength={3}
          />
        </div>
      </div>

      <datalist id="airport-codes">
        {nodesData.map(a => <option key={a.id} value={a.id} />)}
      </datalist>

      <div className="route-modes">
        <button
          type="button"
          className={`route-mode ${mode === 'hops' ? 'active' : ''}`}
          onClick={() => onModeChange('hops')}
        >
          Fewest hops
        </button>
        <button
          type="button"
          className={`route-mode ${mode === 'distance' ? 'active' : ''}`}
          onClick={() => onModeChange('distance')}
        >
          Shortest distance
        </button>
      </div>

      <button
        type="submit"
        className="route-btn"
        disabled={status === 'loading'}
      >
        {status === 'loading' ? 'Routing…' : 'Find Route'}
      </button>

      {status === 'error' && error && (
        <div className="route-result route-error">
          <span className="route-error-title">{ERROR_TITLES[error.code] || 'Error'}</span>
          <span className="route-error-msg">{error.message}</span>
        </div>
      )}

      {status === 'success' && result && (
        <div className="route-result">
          <div className="route-path">{result.path.join('  →  ')}</div>
          <div className="route-meta">
            <span>{result.hops} {result.hops === 1 ? 'hop' : 'hops'}</span>
            <span className="route-algo">{result.algorithm}</span>
            {result.distanceKm != null && (
              <span>{Math.round(result.distanceKm).toLocaleString()} km</span>
            )}
            {latency != null && <span>{latency} ms</span>}
          </div>
        </div>
      )}
    </form>
  );
};

export default RoutePanel;
