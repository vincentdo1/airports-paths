import { fireEvent, render, screen } from '@testing-library/react';
import RoutePanel from './index';

const baseProps = {
  source: '',
  destination: '',
  mode: 'hops',
  onSourceChange: () => {},
  onDestinationChange: () => {},
  onModeChange: () => {},
  status: 'idle',
  result: null,
  error: null,
  latency: null,
  onFindRoute: () => {},
};

test('renders the route search panel', () => {
  render(<RoutePanel {...baseProps} />);
  expect(screen.getByText('Find a Route')).toBeInTheDocument();
});

test('submits through the form unless a route is already loading', () => {
  const onFindRoute = jest.fn();
  const { rerender } = render(<RoutePanel {...baseProps} onFindRoute={onFindRoute} />);
  const form = screen.getByRole('form', { name: 'Route search' });

  fireEvent.submit(form);
  expect(onFindRoute).toHaveBeenCalledTimes(1);

  rerender(<RoutePanel {...baseProps} status="loading" onFindRoute={onFindRoute} />);
  fireEvent.submit(form);
  expect(onFindRoute).toHaveBeenCalledTimes(1);
});

test('shows a friendly title and message for an error', () => {
  render(<RoutePanel {...baseProps} status="error" error={{ code: 'NO_ROUTE', message: 'No route between them' }} />);
  expect(screen.getByText('No route found')).toBeInTheDocument();
  expect(screen.getByText('No route between them')).toBeInTheDocument();
});

test('shows the path and latency on success', () => {
  render(
    <RoutePanel
      {...baseProps}
      status="success"
      result={{ path: ['PEK', 'JFK'], hops: 1, algorithm: 'bfs' }}
      latency={42}
    />
  );
  expect(screen.getByText(/PEK/)).toBeInTheDocument();
  expect(screen.getByText('42 ms')).toBeInTheDocument();
});
