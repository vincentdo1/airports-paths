import React, { useEffect, useState } from 'react';
import Globe from 'react-globe.gl'; // assuming using Globe.gl

const MyGlobe = () => {
  const [airports, setAirports] = useState([]);
  const [paths, setPaths] = useState([]);

  const airportsData = [
    {
      id: "JFK", // Unique identifier for the airport
      name: "John F. Kennedy International Airport",
      lat: 40.6413,
      lng: -73.7781,
      // Any additional data you want to display or use (like country, size, etc.)
    },
    {
      id: "LAX",
      name: "Los Angeles International Airport",
      lat: 33.9416,
      lng: -118.4085,
      // Additional data...
    },
  ];

  const edgesData = [
      [40.6413, -73.7781, 0],
      [33.9416, -118.4085, 0]
  ];

  const arcsData = [
    {
      startLat: 40.6413,
      startLng: -73.7781,
      endLat: 33.9416,
      endLng: -118.4085,
      color: [['red', 'white', 'blue', 'green'][Math.round(Math.random() * 3)], ['red', 'white', 'blue', 'green'][Math.round(Math.random() * 3)]]
    },
  ];

  useEffect(() => {
    setAirports(airportsData);
    setPaths(edgesData);
  }, []);

  console.log('Airports:', airports);
console.log('Paths:', paths);


  return (
    <Globe
      pointsData={airports}
      arcsData={arcsData}
      arcColor={'color'}
      arcDashLength={() => Math.random()}
      arcDashGap={() => Math.random()}
      arcDashAnimateTime={() => Math.random() * 4000 + 500}
      // Props and settings for your globe
      // other globe settings...
    />
  );
};

export default MyGlobe;