import { useState, useCallback, useRef } from 'react';

const API_BASE = process.env.REACT_APP_API_URL || 'http://localhost:8080';
const TIMEOUT_MS = 8000;
const EMPTY = [];

export const useRoute = () => {
  const [source, setSource] = useState('');
  const [destination, setDestination] = useState('');
  const [mode, setMode] = useState('hops');

  const [status, setStatus] = useState('idle');
  const [result, setResult] = useState(null);
  const [error, setError] = useState(null);
  const [latency, setLatency] = useState(null);

  const activeRef = useRef(null);

  const requestRoute = useCallback(async () => {
    const from = source.trim().toUpperCase();
    const to = destination.trim().toUpperCase();

    if (activeRef.current) {
      activeRef.current.abort();
      activeRef.current = null;
    }

    if (!from || !to) {
      setStatus('error');
      setResult(null);
      setError({ code: 'MISSING_PARAMETER', message: 'Enter a source and destination airport.' });
      return;
    }

    const controller = new AbortController();
    activeRef.current = controller;
    const timer = setTimeout(() => controller.abort(), TIMEOUT_MS);

    setStatus('loading');
    setError(null);
    const started = performance.now();
    try {
      const url = `${API_BASE}/api/v1/routes?source=${encodeURIComponent(from)}`
        + `&destination=${encodeURIComponent(to)}&mode=${mode}`;
      const res = await fetch(url, { signal: controller.signal });
      const body = await res.json();
      clearTimeout(timer);
      if (activeRef.current !== controller) {
        return;
      }
      activeRef.current = null;
      setLatency(Math.round(performance.now() - started));
      if (!res.ok) {
        setStatus('error');
        setResult(null);
        setError(body.error || { code: 'ERROR', message: 'The request could not be completed.' });
        return;
      }
      setStatus('success');
      setResult(body);
      setError(null);
    } catch (e) {
      clearTimeout(timer);
      if (activeRef.current !== controller) {
        return;
      }
      activeRef.current = null;
      setLatency(Math.round(performance.now() - started));
      setStatus('error');
      setResult(null);
      if (e.name === 'AbortError') {
        setError({ code: 'TIMEOUT', message: 'The routing service took too long to respond.' });
      } else {
        setError({ code: 'SERVER_UNAVAILABLE', message: 'Could not reach the routing service. Is it running?' });
      }
    }
  }, [source, destination, mode]);

  return {
    source, setSource,
    destination, setDestination,
    mode, setMode,
    status, result, error, latency,
    path: result ? result.path : EMPTY,
    coordinates: result && result.coordinates ? result.coordinates : EMPTY,
    requestRoute,
  };
};
