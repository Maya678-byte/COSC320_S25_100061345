// This function modifies a vector by adding 88 but avoids taking ownership.
// Fix: Accepts a reference to prevent ownership transfer.
#[allow(dead_code)] // Suppresses "unused function" warning if not used in `main()`
fn fill_vec(vec: &Vec<i32>) -> Vec<i32> {
    let mut new_vec = vec.clone(); // Clones the input to avoid modifying the original.
    new_vec.push(88); // Adds 88 to the new vector.
    new_vec // Returns the modified copy.
}

fn main() {
    let vec0 = vec![22, 44, 66]; // Original vector.
    let vec1 = fill_vec(&vec0);   // Pass a reference to avoid moving ownership.
    println!("{:?}", vec1);       // Expected output: [22, 44, 66, 88]
}

#[cfg(test)] // This test module is only compiled when running `cargo test`
mod tests {
    use super::*;

    #[test]
    fn move_semantics2() {
        let vec0 = vec![22, 44, 66]; // Define the original vector.
        let vec1 = fill_vec(&vec0);  // Pass a reference instead of moving ownership.

        // Fix: Now vec0 is still accessible after calling fill_vec()
        assert_eq!(vec0, vec![22, 44, 66]); // Ensures vec0 is unchanged.
        assert_eq!(vec1, vec![22, 44, 66, 88]); // Confirms vec1 has the added value.
    }
}
