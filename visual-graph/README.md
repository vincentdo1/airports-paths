# visual-graph

React/WebGL frontend for the airport network — a globe of the 500 airports, their
routes, and live route search against the C++ API.

```
npm install
npm start     # dev server on :3000
npm test
npm run build
```

`REACT_APP_API_URL` points the build at the routing API; without it the app calls
`http://localhost:8080`. The deploy workflow builds this into `docs/` for GitHub Pages.

Bootstrapped with [Create React App](https://github.com/facebook/create-react-app).
