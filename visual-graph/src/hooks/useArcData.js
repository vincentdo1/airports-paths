import { useState, useEffect, useMemo } from 'react';
import arcsData from '../data/arcs_data.json';
import { sampleArcs } from '../utils/arcSampler';

const DEFAULT_COUNT = 500;
const EMPTY_ARR = [];

export const useArcData = () => {
  const [displayArcs, setDisplayArcs] = useState(() => sampleArcs(arcsData, DEFAULT_COUNT));
  const [showArcs,    setShowArcs]    = useState(true);
  const [arcCount,    setArcCount]    = useState(DEFAULT_COUNT);
  const [sliderValue, setSliderValue] = useState(DEFAULT_COUNT);

  // Re-sample only when arcCount commits (slider mouseup), not on every drag tick.
  useEffect(() => {
    setDisplayArcs(sampleArcs(arcsData, arcCount));
  }, [arcCount]);

  const handleSliderChange = (e) => setSliderValue(Number(e.target.value));
  const handleSliderCommit = (e) => {
    const val = Number(e.target.value);
    setSliderValue(val);
    setArcCount(val);
  };

  const activeArcs = useMemo(
    () => (showArcs ? displayArcs : EMPTY_ARR),
    [showArcs, displayArcs]
  );

  return {
    activeArcs,
    showArcs,
    setShowArcs,
    arcCount,
    sliderValue,
    handleSliderChange,
    handleSliderCommit,
  };
};
