Yes, you can write a similar application in Rust using the `iced` GUI library, which is a cross-platform GUI library for Rust inspired by Elm.

Here is an example of how you could create a moving button in Rust with `iced`:

First, add the necessary dependencies to your `Cargo.toml`:

```toml
[dependencies]
iced = "0.3"
iced_native = "0.3"
iced_wgpu = "0.3"
rand = "0.8"
```

Then, create your `main.rs` file:

```rust
use iced::{
    button, executor, Align, Application, Button, Clipboard, Column, Command, Container, Element,
    Length, Settings, Subscription, Text,
};
use iced_native::{Event, mouse};
use iced_native::event::Status;
use rand::Rng;

#[derive(Default)]
struct MovingButtonApp {
    button_state: button::State,
    button_position: (f32, f32),
    window_size: (f32, f32),
}

#[derive(Debug, Clone, Copy)]
enum Message {
    ButtonPressed,
    MoveButton,
    MouseMoved(f32, f32),
}

impl Application for MovingButtonApp {
    type Executor = executor::Default;
    type Message = Message;
    type Flags = ();

    fn new(_flags: ()) -> (Self, Command<Message>) {
        (Self::default(), Command::none())
    }

    fn title(&self) -> String {
        String::from("Moving Button")
    }

    fn update(&mut self, message: Message, _: &mut Clipboard) -> Command<Message> {
        match message {
            Message::ButtonPressed => {
                println!("Button pressed!");
            }
            Message::MoveButton => {
                let mut rng = rand::thread_rng();
                let dx = rng.gen_range(-10.0..10.0);
                let dy = rng.gen_range(-10.0..10.0);

                self.button_position.0 = (self.button_position.0 + dx).clamp(0.0, self.window_size.0 - 200.0);
                self.button_position.1 = (self.button_position.1 + dy).clamp(0.0, self.window_size.1 - 50.0);
            }
            Message::MouseMoved(x, y) => {
                let button_x = self.button_position.0;
                let button_y = self.button_position.1;

                let dx = x - button_x;
                let dy = y - button_y;
                let dist = (dx * dx + dy * dy).sqrt();

                if dist < 100.0 {
                    let mut rng = rand::thread_rng();
                    let angle = rng.gen_range(0.0..std::f32::consts::PI * 2.0);
                    let speed = 100.0;
                    let new_dx = angle.cos() * speed;
                    let new_dy = angle.sin() * speed;

                    self.button_position.0 = (self.button_position.0 + new_dx).clamp(0.0, self.window_size.0 - 200.0);
                    self.button_position.1 = (self.button_position.1 + new_dy).clamp(0.0, self.window_size.1 - 50.0);
                } else {
                    self.button_position.0 = (self.button_position.0 + dx.signum() * 10.0).clamp(0.0, self.window_size.0 - 200.0);
                    self.button_position.1 = (self.button_position.1 + dy.signum() * 10.0).clamp(0.0, self.window_size.1 - 50.0);
                }
            }
        }

        Command::none()
    }

    fn view(&mut self) -> Element<Message> {
        let button = Button::new(&mut self.button_state, Text::new("Click here to keep\nan idiot busy for hours!"))
            .on_press(Message::ButtonPressed);

        Container::new(button)
            .width(Length::Fill)
            .height(Length::Fill)
            .align_x(iced::Align::Start)
            .align_y(iced::Align::Start)
            .padding(self.button_position)
            .into()
    }

    fn subscription(&self) -> Subscription<Message> {
        iced_native::subscription::events().map(|event| match event {
            Event::Mouse(mouse::Event::CursorMoved { position }) => {
                Message::MouseMoved(position.x, position.y)
            }
            _ => Message::MoveButton,
        })
    }

    fn resized(&mut self, width: u32, height: u32) {
        self.window_size = (width as f32, height as f32);
    }
}

fn main() {
    MovingButtonApp::run(Settings::default()).unwrap();
}
```

This Rust code uses the `iced` library to create a window with a moving button that avoids the mouse cursor. The button moves randomly when the cursor gets too close. The application updates its state based on mouse events and a timer for periodic updates.