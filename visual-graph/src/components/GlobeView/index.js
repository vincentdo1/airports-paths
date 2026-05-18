import React from 'react';
import Globe from 'react-globe.gl';
import { enrichedAirports, topAirports } from '../../utils/enrichAirports';

// Stable function refs defined at module level so React.memo's shallow
// prop comparison is never invalidated by a re-render of the parent.
const LABEL_COLOR = () => 'rgba(255, 220, 50, 0.95)';
const POINT_LABEL = () => '';

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

export default GlobeView;
