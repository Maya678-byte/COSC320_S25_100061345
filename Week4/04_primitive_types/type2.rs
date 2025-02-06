fn main() {
    // Note the _single_ quotes, these are different from double quotes.
    let my_first_initial = 'C';
    
    if my_first_initial.is_alphabetic() {
        println!("Alphabetical!");
    } else if my_first_initial.is_numeric() {
        println!("Numerical!");
    } else {
        println!("Neither alphabetic nor numeric!");
    }

    // Case Arabic letter (example: 'ع') (prints alphabetical)
    //let your_character = 'ع';

//Case Arabic number (prints numerical)
//let your_character = '٣';

//Case emoji (prints neither)
let your_character = '😊';

    if your_character.is_alphabetic() {
        println!("Alphabetical!");
    } else if your_character.is_numeric() {
        println!("Numerical!");
    } else {
        println!("Neither alphabetic nor numeric!");
    }
}
