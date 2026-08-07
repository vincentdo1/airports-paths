import { render, screen } from '@testing-library/react';
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
