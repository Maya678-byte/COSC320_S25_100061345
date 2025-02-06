fn main() {
    let cat = ("Furry McFurson", 3.5);

    // Destructure the tuple into `name` and `age`
    let (name, age) = cat;

    println!("{name} is {age} years old");
}
