import React from 'react';
import { TOP_AIRPORTS, AIRPORT_NAMES } from '../../constants/airports';
import { enrichedAirports } from '../../utils/enrichAirports';
import './InfoCard.css';

const InfoCard = ({ selectedAirport }) => {
  if (!selectedAirport) return null;

  const data = enrichedAirports.find(a => a.id === selectedAirport);
  if (!data) return null;

  const rank = TOP_AIRPORTS.indexOf(selectedAirport);

  return (
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
          <span className="coord-value">{data.lat.toFixed(3)}°</span>
        </div>
        <div className="coord-item">
          <span className="coord-label">LNG</span>
          <span className="coord-value">{data.lng.toFixed(3)}°</span>
        </div>
      </div>

      {rank >= 0 && (
        <p className="info-bc-note">
          High betweenness centrality — a critical routing hub in the global flight network.
        </p>
      )}
    </div>
  );
};

export default InfoCard;
