import React from 'react';
import { TOP_AIRPORTS } from '../../constants/airports';
import './Tooltip.css';

const Tooltip = React.forwardRef(({ hoveredAirport }, ref) => (
  <div
    ref={ref}
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
));

Tooltip.displayName = 'Tooltip';

export default Tooltip;
