# The Anti-User Button

### Overview

This project showcases an innovative implementation of a button that moves away from the cursor constantly, using various programming languages, this code was created with the assistance of ChatGPT 3.5.

### Demo

- **HTML Version**: Open `button.html` in a browser to see the high-speed button in action.

### Additional Implementations

- **Excel Version**: A fun version that can be embedded in a Microsoft Excel spreadsheet.
- **Multi-Language Support**: Code snippets available for several other languages to achieve the same unclickable button effect.

### Python Implementation

The Python script displays a button in a window that continuously moves away, making it impossible to click.

### Development Journey

I spent a few hours collaborating with ChatGPT, iterating through multiple versions before finalizing the script. After generating the Python script, I used ChatGPT to rewrite it in HTML/JavaScript and several other languages. Rewriting the code in other programming languages only took a few minutes.
The code was generated with the help of ChatGPT 3.5 -Free Version - 2024.

---

```html
<!-- button.html -->
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Unclickable Button</title>
    <style>
        #button {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
        }
    </style>
</head>
<body>
    <button id="button" onclick="alert('You clicked me!')">Click Me!</button>
    <script>
        const button = document.getElementById('button');
        button.addEventListener('mouseover', () => {
            const x = Math.random() * (window.innerWidth - button.clientWidth);
            const y = Math.random() * (window.innerHeight - button.clientHeight);
            button.style.left = `${x}px`;
            button.style.top = `${y}px`;
        });
    </script>
</body>
</html>
```

### Python Code Snippet

```python
import tkinter as tk
import random

def move_button():
    button.place(x=random.randint(0, window.winfo_width() - button.winfo_width()),
                 y=random.randint(0, window.winfo_height() - button.winfo_height()))

window = tk.Tk()
window.title("Unclickable Button")
window.geometry("300x300")

button = tk.Button(window, text="Click Me!")
button.place(x=100, y=100)
button.bind("<Enter>", lambda e: move_button())

window.mainloop()
```

Explore and enjoy the unclickable button in various environments!
