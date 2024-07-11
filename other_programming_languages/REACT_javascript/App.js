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
