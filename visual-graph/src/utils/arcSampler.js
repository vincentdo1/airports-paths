const ARC_COLOR = ['rgba(0, 180, 255, 0.65)', 'rgba(0, 60, 180, 0.08)'];

export const sampleArcs = (data, n) => {
  const shuffled = [...data].sort(() => 0.5 - Math.random());
  return shuffled.slice(0, n).map(arc => ({
    ...arc,
    arcColor:    ARC_COLOR,
    dashLength:  Math.random() * 0.4 + 0.1,
    dashGap:     Math.random() * 0.4 + 0.1,
    animateTime: Math.random() * 4000 + 1000,
  }));
};
