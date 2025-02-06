#[derive(Debug)]
enum Message {
    Resize,
    Move,
    Echo,
    ChangeColor,
    Quit,
}

fn main() {
    println!("{:?}", Message::Resize); // Prints: Resize
    println!("{:?}", Message::Move); // Prints: Move
    println!("{:?}", Message::Echo); // Prints: Echo
    println!("{:?}", Message::ChangeColor); // Prints: ChangeColor
    println!("{:?}", Message::Quit); // Prints: Quit
}
