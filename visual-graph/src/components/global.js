import React, { useEffect, useState, useRef } from 'react';
import Globe from 'react-globe.gl';
import nodesData from '../resources/nodes_data.json';
import arcsData from '../resources/arcs_data.json';
import './global.css'

const MyGlobe = () => {
  const [airports, setAirports] = useState([]);
  const [selectedAirport, setSelectedAirport] = useState('');
  const globeEl = useRef(null);

  // List of top ten airports
  const topTenAirports = ['PEK', 'IST', 'SVO', 'DEL', 'URC', 'ICN', 'DXB', 'KUL', 'CCU', 'YYZ'];

  useEffect(() => {
    const airportsWithColor = nodesData.map(airport => {
      // Check if the airport is in the top ten list
      if (topTenAirports.includes(airport.id)) {
        // Mark this airport with a distinct color or size
        return {
          ...airport,
          color: 'red',
          size: 0.5,
        };
      }
      return {
        ...airport,
        color: 'white',
      };
    });
    setAirports(airportsWithColor);
  }, []);

  const goToAirport = (airportID) => {
    const airport = airports.find(a => a.id === airportID);
    if (airport && globeEl.current) {
      globeEl.current.pointOfView({
        lat: airport.lat,
        lng: airport.lng,
        altitude: 2.5
      }, 1000);
    }
  };

  useEffect(() => {
    if (selectedAirport) {
      goToAirport(selectedAirport);
    }
  }, [selectedAirport]);

  return (
    <div className="globe-container">
  <select
    className="globe-search"
    value={selectedAirport}
    onChange={(e) => setSelectedAirport(e.target.value)}
  >
        <option value="">Select an Airport</option>
        {topTenAirports.map(airport => (
          <option key={airport} value={airport}>{airport}</option>
        ))}
      </select>
      <Globe
        ref={globeEl}
        globeImageUrl="//unpkg.com/three-globe/example/img/earth-night.jpg"
        pointsData={airports}
        pointColor="color"
        pointAltitude="size"
        arcsData={arcsData}
        arcColor={'color'}
        arcDashLength={() => Math.random()}
        arcDashGap={() => Math.random()}
        arcDashAnimateTime={() => Math.random() * 4000 + 500}
      />
    </div>
  );
};

export default MyGlobe;
