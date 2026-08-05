import React, { useMemo } from 'react';
import Globe from 'react-globe.gl';
import { enrichedAirports, topAirports } from '../../utils/enrichAirports';

// Stable function refs defined at module level so React.memo's shallow
// prop comparison is never invalidated by a re-render of the parent.
const LABEL_COLOR = () => 'rgba(255, 220, 50, 0.95)';
const POINT_LABEL = () => '';
// Background network arcs have no stroke of their own; a computed route sets one
// so it draws thicker than the surrounding traffic.
const ARC_STROKE = (arc) => arc.stroke || 0.5;

const GlobeView = React.memo(({ globeEl, activeArcs, routeArcs, onPointHover, onPointClick }) => {
  // Draw the computed route on top of the sampled network arcs.
  const arcs = useMemo(
    () => (routeArcs && routeArcs.length ? [...activeArcs, ...routeArcs] : activeArcs),
    [activeArcs, routeArcs]
  );

  return (
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
      arcsData={arcs}
      arcColor="arcColor"
      arcDashLength="dashLength"
      arcDashGap="dashGap"
      arcDashAnimateTime="animateTime"
      arcStroke={ARC_STROKE}
      arcTransitionDuration={0}
    />
  );
});

GlobeView.displayName = 'GlobeView';

export default GlobeView;
