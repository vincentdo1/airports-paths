import { renderHook, act } from '@testing-library/react';
import { useRoute } from './useRoute';

const originalFetch = global.fetch;

function mockFetch(ok, body) {
  global.fetch = jest.fn().mockResolvedValueOnce({ ok, json: async () => body });
}

afterEach(() => {
  jest.restoreAllMocks();
  if (originalFetch) {
    global.fetch = originalFetch;
  } else {
    delete global.fetch;
  }
});

test('requires a source and destination', async () => {
  const { result } = renderHook(() => useRoute());
  await act(async () => { await result.current.requestRoute(); });
  expect(result.current.status).toBe('error');
  expect(result.current.error.code).toBe('MISSING_PARAMETER');
});

test('exposes the route and coordinates on success', async () => {
  mockFetch(true, {
    source: 'PEK',
    destination: 'JFK',
    mode: 'hops',
    algorithm: 'bfs',
    path: ['PEK', 'JFK'],
    hops: 1,
    coordinates: [{ code: 'PEK', lat: 40, lng: 116 }, { code: 'JFK', lat: 40, lng: -73 }],
  });
  const { result } = renderHook(() => useRoute());
  act(() => {
    result.current.setSource('PEK');
    result.current.setDestination('JFK');
  });
  await act(async () => { await result.current.requestRoute(); });
  expect(result.current.status).toBe('success');
  expect(result.current.result.path).toEqual(['PEK', 'JFK']);
  expect(result.current.coordinates).toHaveLength(2);
});

test('surfaces API errors from the response body', async () => {
  mockFetch(false, { error: { code: 'UNKNOWN_AIRPORT', message: 'not found' } });
  const { result } = renderHook(() => useRoute());
  act(() => {
    result.current.setSource('PEK');
    result.current.setDestination('ZZZ');
  });
  await act(async () => { await result.current.requestRoute(); });
  expect(result.current.status).toBe('error');
  expect(result.current.error.code).toBe('UNKNOWN_AIRPORT');
});

test('reports the service as unavailable when fetch rejects', async () => {
  global.fetch = jest.fn().mockRejectedValueOnce(new TypeError('network down'));
  const { result } = renderHook(() => useRoute());
  act(() => {
    result.current.setSource('PEK');
    result.current.setDestination('JFK');
  });
  await act(async () => { await result.current.requestRoute(); });
  expect(result.current.status).toBe('error');
  expect(result.current.error.code).toBe('SERVER_UNAVAILABLE');
});

test('treats a success without coordinates as an empty list', async () => {
  mockFetch(true, { source: 'PEK', destination: 'JFK', mode: 'hops', path: ['PEK', 'JFK'], hops: 1 });
  const { result } = renderHook(() => useRoute());
  act(() => {
    result.current.setSource('PEK');
    result.current.setDestination('JFK');
  });
  await act(async () => { await result.current.requestRoute(); });
  expect(result.current.status).toBe('success');
  expect(result.current.coordinates).toEqual([]);
});

test('keeps validation errors when an older request finishes', async () => {
  let resolveFetch;
  global.fetch = jest.fn(() => new Promise((resolve) => {
    resolveFetch = resolve;
  }));

  const { result } = renderHook(() => useRoute());
  act(() => {
    result.current.setSource('PEK');
    result.current.setDestination('JFK');
  });

  let pendingRequest;
  await act(async () => {
    pendingRequest = result.current.requestRoute();
  });

  act(() => {
    result.current.setDestination('');
  });
  await act(async () => { await result.current.requestRoute(); });

  expect(global.fetch.mock.calls[0][1].signal.aborted).toBe(true);
  expect(result.current.status).toBe('error');
  expect(result.current.error.code).toBe('MISSING_PARAMETER');

  await act(async () => {
    resolveFetch({
      ok: true,
      json: async () => ({
        path: ['PEK', 'JFK'],
        coordinates: [],
        hops: 1,
        algorithm: 'bfs',
      }),
    });
    await pendingRequest;
  });

  expect(result.current.status).toBe('error');
  expect(result.current.error.code).toBe('MISSING_PARAMETER');
});
