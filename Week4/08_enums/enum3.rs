// Define a struct to represent a 2D point with `x` and `y` coordinates.
struct Point {
    x: u64,
    y: u64,
}

// Define an enum `Message` with different types of messages that can be processed.
enum Message {
    Resize { width: u64, height: u64 },
    Move(Point),
    Echo(String),
    ChangeColor(u8, u8, u8),
    Quit,
}

// Define the `State` struct which holds the state of a system.
struct State {
    width: u64,
    height: u64,
    position: Point,
    message: String,
    color: (u8, u8, u8),
    quit: bool,
}

// Implement methods on the `State` struct.
impl State {
    // Method to resize the `State` dimensions (width and height).
    fn resize(&mut self, width: u64, height: u64) {
        self.width = width;
        self.height = height;
    }

    // Method to update the position (`x` and `y`) of `State`.
    fn move_position(&mut self, point: Point) {
        self.position = point;
    }

    // Method to update the message stored in `State`.
    fn echo(&mut self, s: String) {
        self.message = s;
    }

    // Method to change the color of `State` to RGB values.
    fn change_color(&mut self, red: u8, green: u8, blue: u8) {
        self.color = (red, green, blue);
    }

    // Method to mark the `State` as "quit".
    fn quit(&mut self) {
        self.quit = true;
    }

    // Method to process messages and update the state accordingly.
    fn process(&mut self, message: Message) {
        // Match the message and call the appropriate method to update `State`.
        match message {
            Message::Resize { width, height } => self.resize(width, height),
            Message::Move(point) => self.move_position(point),
            Message::Echo(msg) => self.echo(msg),
            Message::ChangeColor(r, g, b) => self.change_color(r, g, b),
            Message::Quit => self.quit(),
        }
    }
}

fn main() {
    // Create a Point instance to use for the `Move` message.
    let point = Point { x: 10, y: 20 };
    
    // Create a State instance to represent the initial state.
    let mut state = State {
        width: 0,
        height: 0,
        position: Point { x: 0, y: 0 },
        message: String::from("hello world"),
        color: (0, 0, 0),
        quit: false,
    };

    // Process different messages to modify the state.
    state.process(Message::Resize { width: 10, height: 30 });
    state.process(Message::Move(point)); // Move to (10, 20)
    state.process(Message::Echo(String::from("Hello, world!")));
    state.process(Message::ChangeColor(255, 0, 0)); // Red color
    state.process(Message::Quit);

    // Print out the updated state.
    println!("State after processing messages:");
    println!("Width: {}", state.width);
    println!("Height: {}", state.height);
    println!("Position: ({}, {})", state.position.x, state.position.y);
    println!("Message: {}", state.message);
    println!("Color: {:?}", state.color);
    println!("Quit: {}", state.quit);
}

#[cfg(test)]
mod tests {
    use super::*;

    // Unit test to ensure the `State` is updated correctly when messages are processed.
    #[test]
    fn test_match_message_call() {
        // Initialize the `State` with default values.
        let mut state = State {
            width: 0,
            height: 0,
            position: Point { x: 0, y: 0 },
            message: String::from("hello world"),
            color: (0, 0, 0),
            quit: false,
        };

        // Process various `Message` variants to update the `State`.
        state.process(Message::Resize { width: 10, height: 30 });
        state.process(Message::Move(Point { x: 10, y: 15 }));
        state.process(Message::Echo(String::from("Hello world!")));
        state.process(Message::ChangeColor(255, 0, 255));
        state.process(Message::Quit);

        // Assert that the state has been updated correctly.
        assert_eq!(state.width, 10);
        assert_eq!(state.height, 30);
        assert_eq!(state.position.x, 10);
        assert_eq!(state.position.y, 15);
        assert_eq!(state.message, "Hello world!");
        assert_eq!(state.color, (255, 0, 255));
        assert!(state.quit); // The `quit` flag should be set to true.
    }
}
