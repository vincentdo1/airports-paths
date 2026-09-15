import { fireEvent, render, screen } from '@testing-library/react';
import ControlPanel from './index';

const baseProps = {
  selectedAirport: '',
  onAirportChange: () => {},
  showArcs: true,
  onToggleArcs: () => {},
  sliderValue: 500,
  arcCount: 500,
  onSliderChange: () => {},
  onSliderCommit: () => {},
};

test('commits route-count changes made with the keyboard', () => {
  const onSliderChange = jest.fn();
  const onSliderCommit = jest.fn();
  render(
    <ControlPanel
      {...baseProps}
      onSliderChange={onSliderChange}
      onSliderCommit={onSliderCommit}
    />
  );

  const slider = screen.getByRole('slider');
  fireEvent.change(slider, { target: { value: '600' } });
  fireEvent.keyUp(slider, { key: 'ArrowRight' });

  expect(onSliderChange).toHaveBeenCalledTimes(1);
  expect(onSliderCommit).toHaveBeenCalledTimes(1);
});
