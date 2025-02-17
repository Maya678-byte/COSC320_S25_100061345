#![allow(clippy::ptr_arg)]

// TODO: Fix the compiler errors without changing anything except adding or
// removing references (the character `&`).

// Shouldn't take ownership
//Therefore it needs to be used as a reference
fn get_char(data: &String) -> char {
    data.chars().last().unwrap()
}

// Should take ownership
//Therefore it shouldn't be referenced
fn string_uppercase(mut data: String) {
    data = data.to_uppercase();

    println!("{data}");
}

fn main() {
    let data = "Rust is great!".to_string();

//Switch &s here due to change in reference
    get_char(&data);

    string_uppercase(data);
}