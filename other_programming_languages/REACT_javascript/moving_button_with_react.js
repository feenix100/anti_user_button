Certainly! Another programming language and environment you can use that is platform-independent is JavaScript with a framework like React for building web applications. Here’s an example using React:

### JavaScript (with React)

1. **Create a new React application** (if you don't already have one):
   ```bash
   npx create-react-app moving-button-app
   cd moving-button-app
   ```

2. **Replace the content of `src/App.js`** with the following code:

```javascript
import React, { useState, useEffect } from 'react';
import './App.css';

const App = () => {
  const [buttonPosition, setButtonPosition] = useState({ left: window.innerWidth / 2, top: window.innerHeight / 2 });
  const [windowSize, setWindowSize] = useState({ width: window.innerWidth, height: window.innerHeight });
  const speed = 100;
  const mouseNearThreshold = 100;
  const interval = 50;

  useEffect(() => {
    const handleResize = () => {
      setWindowSize({ width: window.innerWidth, height: window.innerHeight });
    };

    window.addEventListener('resize', handleResize);

    return () => {
      window.removeEventListener('resize', handleResize);
    };
  }, []);

  useEffect(() => {
    const handleMouseMove = (event) => {
      const { clientX: x, clientY: y } = event;
      const { left: x0, top: y0 } = buttonPosition;
      const dx = x - x0;
      const dy = y - y0;

      const dist = Math.sqrt(dx * dx + dy * dy);

      if (dist < mouseNearThreshold) {
        const angle = Math.random() * 2 * Math.PI;
        setButtonPosition((prevPos) => ({
          left: Math.max(0, Math.min(windowSize.width - 200, prevPos.left + Math.cos(angle) * speed)),
          top: Math.max(0, Math.min(windowSize.height - 50, prevPos.top + Math.sin(angle) * speed)),
        }));
      } else {
        const distX = windowSize.width / 2 - x0;
        const distY = windowSize.height / 2 - y0;
        const centerDist = Math.sqrt(distX * distX + distY * distY);
        if (centerDist > 0) {
          setButtonPosition((prevPos) => ({
            left: Math.max(0, Math.min(windowSize.width - 200, prevPos.left + (distX / centerDist) * speed)),
            top: Math.max(0, Math.min(windowSize.height - 50, prevPos.top + (distY / centerDist) * speed)),
          }));
        }
      }
    };

    window.addEventListener('mousemove', handleMouseMove);

    return () => {
      window.removeEventListener('mousemove', handleMouseMove);
    };
  }, [buttonPosition, windowSize]);

  useEffect(() => {
    const intervalId = setInterval(() => {
      const { left: x0, top: y0 } = buttonPosition;
      const distX = windowSize.width / 2 - x0;
      const distY = windowSize.height / 2 - y0;
      const dist = Math.sqrt(distX * distX + distY * distY);

      if (dist > 1) {
        setButtonPosition((prevPos) => ({
          left: Math.max(0, Math.min(windowSize.width - 200, prevPos.left + (distX / dist) * (speed / 10))),
          top: Math.max(0, Math.min(windowSize.height - 50, prevPos.top + (distY / dist) * (speed / 10))),
        }));
      }
    }, interval);

    return () => clearInterval(intervalId);
  }, [buttonPosition, windowSize]);

  return (
    <div className="App">
      <button
        style={{
          position: 'absolute',
          left: `${buttonPosition.left}px`,
          top: `${buttonPosition.top}px`,
          cursor: 'pointer',
        }}
        onClick={(e) => e.preventDefault()}
      >
        Click here to keep<br />an idiot busy for hours!
      </button>
    </div>
  );
};

export default App;
```

3. **Add some basic styling** by creating a `src/App.css` file and adding:

```css
body, html, #root, .App {
  height: 100%;
  margin: 0;
  display: flex;
  justify-content: center;
  align-items: center;
}
button {
  width: 200px;
  height: 50px;
}
```

This React application creates a button that moves away from the mouse cursor and towards the center of the window if the cursor is far. The button's position is updated based on mouse movement and periodically adjusted to move back towards the center. React's useState and useEffect hooks manage state and side effects respectively.