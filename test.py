import tkinter as tk
# created by block burner 4/01/23
# this script was created using chat gpt free version
# it creates a button that will constantly move away from the mouse cursor
# the user should never be able to click on the button


class MovingButton(tk.Button):
    def __init__(self, master, **kwargs):
        # Call the __init__ method of the parent class (tk.Button)
        super().__init__(master, **kwargs)
        # Initialize the starting position of the button at the center of the window
        self.start_x = master.winfo_width() // 2
        self.start_y = master.winfo_height() // 2
        # Place the button at the starting position
        self.place(x=self.start_x, y=self.start_y)
        # Bind the <Motion> event to the move_button method
        self.bind("<Motion>", self.move_button)
        # Bind the <Button-1> event to a lambda function that returns "break"
        # This prevents the button from being dragged when clicked
        self.bind("<Button-1>", lambda event: "break")


# this function should move the button when the mouse cursor get close to it
    def move_button(self, event):
        # Get the current position of the mouse pointer
        x, y = event.x, event.y
        # Get the coordinates of the button in the window
        x0, y0, x1, y1 = self.master.bbox(self)
        # Calculate the distance to move the button in the x and y directions
        dx, dy = x - x0, y - y0
        # Limit the movement to the boundaries of the window
        if dx < 0:
            dx = max(dx, -x0)
        elif dx > 0:
            dx = min(dx, self.master.winfo_width() - x1)
        if dy < 0:
            dy = max(dy, -y0)
        elif dy > 0:
            dy = min(dy, self.master.winfo_height() - y1)
        # Calculate the distance between the new button position and the starting position
        dist_x = self.start_x - (x + dx)
        dist_y = self.start_y - (y + dy)
        dist = (dist_x**2 + dist_y**2)**0.5
        # Move the button with a constant speed towards the new position
        if dist > 0:
            speed = 100
            dx *= speed / dist
            dy *= speed / dist
            self.place(x=x+dx, y=y+dy)
        # If the button has reached the starting position, place it there
        else:
            self.place(x=self.start_x, y=self.start_y)

# Create the main window
root = tk.Tk()
root.geometry("800x800")

# Create a MovingButton object and place it in the center of the window
button = MovingButton(root, text="Click here to keep\nan idiot busy for hours!")
button.place(relx=0.5, rely=0.5, anchor="center")

# Start the event loop
root.mainloop()

