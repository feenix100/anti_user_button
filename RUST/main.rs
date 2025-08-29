use iced::{
    executor, Application, Command, Element, Length, Settings, Subscription, Theme,
};
use iced::widget::{Button, Column, Container, Row, Space, Text, button};
use iced::event::Event;
use iced::{mouse, window, subscription};

use rand::Rng;

#[derive(Default)]
struct MovingButtonApp {
    // New Iced: no `button::State` field needed; the widget is stateless on your side.
    button_position: (f32, f32), // (x, y)
    window_size: (f32, f32),     // (w, h)
}

#[derive(Debug, Clone, Copy)]
enum Message {
    ButtonPressed,
    MoveButton,
    MouseMoved(f32, f32),
    Resized(u32, u32),
}

impl Application for MovingButtonApp {
    type Executor = executor::Default;
    type Message = Message;
    type Flags = ();
    type Theme = Theme;

    fn new(_flags: ()) -> (Self, Command<Message>) {
        (
            Self {
                window_size: (800.0, 600.0), // initial guess; updated on resize
                ..Self::default()
            },
            Command::none(),
        )
    }

    fn title(&self) -> String {
        String::from("Moving Button")
    }

    fn update(&mut self, message: Message) -> Command<Message> {
        match message {
            Message::ButtonPressed => {
                println!("Button pressed!");
            }
            Message::MoveButton => {
                let mut rng = rand::thread_rng();
                let dx = rng.gen_range(-10.0..10.0);
                let dy = rng.gen_range(-10.0..10.0);

                self.button_position.0 = (self.button_position.0 + dx)
                    .clamp(0.0, (self.window_size.0 - 200.0).max(0.0));
                self.button_position.1 = (self.button_position.1 + dy)
                    .clamp(0.0, (self.window_size.1 - 50.0).max(0.0));
            }
            Message::MouseMoved(x, y) => {
                let (bx, by) = self.button_position;
                let dx = x - bx;
                let dy = y - by;
                let dist = (dx * dx + dy * dy).sqrt();

                if dist < 100.0 {
                    let mut rng = rand::thread_rng();
                    let angle = rng.gen_range(0.0..std::f32::consts::PI * 2.0);
                    let speed = 100.0;
                    let ndx = angle.cos() * speed;
                    let ndy = angle.sin() * speed;

                    self.button_position.0 = (self.button_position.0 + ndx)
                        .clamp(0.0, (self.window_size.0 - 200.0).max(0.0));
                    self.button_position.1 = (self.button_position.1 + ndy)
                        .clamp(0.0, (self.window_size.1 - 50.0).max(0.0));
                } else {
                    self.button_position.0 = (self.button_position.0 + dx.signum() * 10.0)
                        .clamp(0.0, (self.window_size.0 - 200.0).max(0.0));
                    self.button_position.1 = (self.button_position.1 + dy.signum() * 10.0)
                        .clamp(0.0, (self.window_size.1 - 50.0).max(0.0));
                }
            }
            Message::Resized(w, h) => {
                self.window_size = (w as f32, h as f32);
                // keep the button inside bounds after resize
                self.button_position.0 = self.button_position.0.clamp(0.0, (self.window_size.0 - 200.0).max(0.0));
                self.button_position.1 = self.button_position.1.clamp(0.0, (self.window_size.1 - 50.0).max(0.0));
            }
        }

        Command::none()
    }

    fn view(&self) -> Element<Message> {
        let button = Button::new(Text::new("Click here to keep\nan idiot busy for hours!"))
            .on_press(Message::ButtonPressed);

        // Position using spacer widgets:
        // Top spacer = y, Left spacer = x.
        // In modern Iced, use Length::Fixed(f32) instead of deprecated Units.
        let x = self.button_position.0.max(0.0);
        let y = self.button_position.1.max(0.0);

        let content = Column::new()
            .push(Space::with_height(Length::Fixed(y)))
            .push(
                Row::new()
                    .push(Space::with_width(Length::Fixed(x)))
                    .push(button),
            );

        Container::new(content)
            .width(Length::Fill)
            .height(Length::Fill)
            .into()
    }

    fn subscription(&self) -> Subscription<Message> {
        subscription::events().map(|event| match event {
            // Recent Iced emits cursor position as a point:
            Event::Mouse(mouse::Event::CursorMoved { position, .. }) => {
                Message::MouseMoved(position.x, position.y)
            }
            Event::Window(window::Event::Resized { width, height }) => {
                Message::Resized(width, height)
            }
            _ => Message::MoveButton,
        })
    }
}

fn main() -> iced::Result {
    MovingButtonApp::run(Settings::default())
}
