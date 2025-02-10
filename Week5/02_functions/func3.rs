fn call_me(num: u8) {
//u8 is unsigned and can only take positive numbers
    for i in 0..num {
        println!("Ring! Call number {}", i + 1);
    }
}

fn main() {
    // TODO: Fix the function call.
    call_me(4);
}