import { useState, useCallback } from 'react';

// Where the C++ routing service lives. Defaults to a local server; set
// REACT_APP_API_URL at build time when the API is hosted somewhere else.
const API_BASE = process.env.REACT_APP_API_URL || 'http://localhost:8080';

const EMPTY_PATH = [];

export const useRoute = () => {
  const [source, setSource]           = useState('');
  const [destination, setDestination] = useState('');
  const [mode, setMode]               = useState('hops');

  const [status, setStatus]   = useState('idle');   // idle | loading | success | error
  const [result, setResult]   = useState(null);     // { path, hops, algorithm, distanceKm? }
  const [error, setError]     = useState(null);     // { code, message }
  const [latency, setLatency] = useState(null);     // round-trip ms

  const requestRoute = useCallback(async () => {
    const from = source.trim().toUpperCase();
    const to   = destination.trim().toUpperCase();
    if (!from || !to) {
      setStatus('error');
      setResult(null);
      setError({ code: 'MISSING_PARAMETER', message: 'Enter a source and destination airport.' });
      return;
    }

    setStatus('loading');
    setError(null);
    const started = performance.now();
    try {
      const url = `${API_BASE}/api/v1/routes?source=${encodeURIComponent(from)}`
                + `&destination=${encodeURIComponent(to)}&mode=${mode}`;
      const res = await fetch(url);
      const body = await res.json();
      setLatency(Math.round(performance.now() - started));

      if (!res.ok) {
        // The service returns { error: { code, message } } for every failure.
        setStatus('error');
        setResult(null);
        setError(body.error || { code: 'ERROR', message: 'The request could not be completed.' });
        return;
      }
      setStatus('success');
      setResult(body);
      setError(null);
    } catch (e) {
      // fetch only rejects when the service can't be reached at all.
      setLatency(Math.round(performance.now() - started));
      setStatus('error');
      setResult(null);
      setError({ code: 'SERVER_UNAVAILABLE', message: 'Could not reach the routing service. Is the server running?' });
    }
  }, [source, destination, mode]);

  return {
    source, setSource,
    destination, setDestination,
    mode, setMode,
    status, result, error, latency,
    path: result ? result.path : EMPTY_PATH,
    requestRoute,
  };
};
