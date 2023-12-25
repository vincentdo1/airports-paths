import React, { useEffect, useState, useRef, useCallback } from 'react';
import debounce from 'lodash.debounce';
import Globe from 'react-globe.gl';
import nodesData from '../resources/nodes_data.json';
import arcsData from '../resources/arcs_data.json';
import './global.css'

const MyGlobe = () => {
  const [airports, setAirports] = useState([]);
  const [selectedAirport, setSelectedAirport] = useState('');
  const [displayArcs, setDisplayArcs] = useState([]);
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

  useEffect(() => {
    // Function to pick 2000 random arcs so not rendered slowly
    const pickRandomArcs = (data, num) => {
      const shuffled = [...data].sort(() => 0.5 - Math.random());
      return shuffled.slice(0, num);
    };

    setDisplayArcs(pickRandomArcs(arcsData, 2000));
  }, []);

  const goToAirport = useCallback(debounce((airportID) => {
    const airport = airports.find(a => a.id === airportID);
    if (airport && globeEl.current) {
      globeEl.current.pointOfView({
        lat: airport.lat,
        lng: airport.lng,
        altitude: 2.5
      }, 2000); // Duration increased for smoother transition
    }
  }, 200), [airports]);

  useEffect(() => {
    if (selectedAirport) {
      goToAirport(selectedAirport);
    }
  }, [selectedAirport, goToAirport]);

  return (
    <div className="globe-container">
      <div className="top-bar">
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
      <a
        href="https://github.com/vincentdo1/airports-paths"
        target="_blank"
        rel="noopener noreferrer"
        className="github-link"
      >
        View on GitHub
      </a>
    </div>
      <Globe
        ref={globeEl}
        globeImageUrl="//unpkg.com/three-globe/example/img/earth-night.jpg"
        pointsData={airports}
        pointColor="color"
        pointAltitude="size"
        arcsData={displayArcs}
        arcColor={'color'}
        arcDashLength={() => Math.random()}
        arcDashGap={() => Math.random()}
        arcDashAnimateTime={() => Math.random() * 4000 + 500}
      />
    </div>
  );
};

export default MyGlobe;
