#[derive(Debug)]
// Define a struct `Point` with two fields `x` and `y` of type `u64`.
struct Point {
    x: u64,
    y: u64,
}

// Define an enum `Message` with different variants, each with data.
#[derive(Debug)]
enum Message {
    // Variant `Resize` includes two fields: `width` and `height` of type `u64`.
    Resize { width: u64, height: u64 },

    // Variant `Move` contains a `Point` struct, which has `x` and `y` fields.
    Move(Point),

    // Variant `Echo` holds a `String`.
    Echo(String),

    // Variant `ChangeColor` contains three `u8` values representing RGB colors.
    ChangeColor(u8, u8, u8),

    // Variant `Quit` has no data, it's just an empty unit variant.
    Quit,
}

// Implement the `Message` enum with a method `call` to handle each variant.
impl Message {
    // Define the `call` method for printing information about each variant.
    fn call(&self) {
        match *self { // Match on the `self` enum to destructure and use the data in each variant.
            Message::Resize { width, height } => {
                // Print the width and height when the `Resize` variant is matched.
                println!("Resizing to width: {}, height: {}", width, height);
            }
            Message::Move(Point { x, y }) => {
                // Print the coordinates (x, y) from the `Point` when the `Move` variant is matched.
                println!("Moving to position x: {}, y: {}", x, y);
            }
            Message::Echo(ref message) => {
                // Print the message when the `Echo` variant is matched.
                println!("Echoing message: {}", message);
            }
            Message::ChangeColor(r, g, b) => {
                // Print the RGB values when the `ChangeColor` variant is matched.
                println!("Changing color to RGB({}, {}, {})", r, g, b);
            }
            Message::Quit => {
                // Just print "Quit" when the `Quit` variant is matched.
                println!("Quit");
            }
        }
    }
}

fn main() {
    // Create an array of `Message` enum variants to demonstrate usage.
    let messages = [
        // Creating a `Resize` message with width and height.
        Message::Resize {
            width: 10,
            height: 30,
        },
        // Creating a `Move` message with a `Point` struct (x = 10, y = 15).
        Message::Move(Point { x: 10, y: 15 }),
        // Creating an `Echo` message with a string.
        Message::Echo(String::from("hello world")),
        // Creating a `ChangeColor` message with RGB values.
        Message::ChangeColor(200, 255, 255),
        // Creating a `Quit` message with no data.
        Message::Quit,
    ];

    // Iterate over the array of messages and call the `call` method on each message.
    for message in &messages {
        message.call(); // This will print the details of each message variant.
    }
}
