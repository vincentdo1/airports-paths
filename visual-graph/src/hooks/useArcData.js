import { useState, useMemo } from 'react';
import arcsData from '../data/arcs_data.json';
import { sampleArcs } from '../utils/arcSampler';

const DEFAULT_COUNT = 500;
const EMPTY_ARCS = [];

export const useArcData = () => {
  const [showArcs, setShowArcs] = useState(true);
  const [arcCount, setArcCount] = useState(DEFAULT_COUNT);
  const [sliderValue, setSliderValue] = useState(DEFAULT_COUNT);

  const displayArcs = useMemo(() => sampleArcs(arcsData, arcCount), [arcCount]);

  const handleSliderChange = (e) => setSliderValue(Number(e.target.value));
  const handleSliderCommit = (e) => {
    const val = Number(e.target.value);
    setSliderValue(val);
    setArcCount(val);
  };

  const activeArcs = showArcs ? displayArcs : EMPTY_ARCS;

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
