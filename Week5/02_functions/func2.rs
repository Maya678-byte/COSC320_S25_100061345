// TODO: Add the missing type of the argument `num` after the colon `:`.
fn call_me(num: i32) {
//i32 is a signed integer: can take both positive or nagative numbers
    for i in 0..num {
        println!("Ring! Call number {}", i + 1);
    }
}

fn main() {
    call_me(3);
}
