fn fill_vec(mut vec: Vec<i32>) -> Vec<i32> {
    // The 'mut' keyword allows 'vec' to be modified within this function.
    vec.push(88); // Add 88 to the vector.

    vec // Return the modified vector.
}

fn main() {
    let vec0 = vec![22, 44, 66]; // Define a vector with initial values.
    let vec1 = fill_vec(vec0);   // Call fill_vec to modify and return the vector.
    println!("{:?}", vec1);      // Print the modified vector.
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn move_semantics1() {
        let vec0 = vec![22, 44, 66]; // Create a vector with initial values.
        let vec1 = fill_vec(vec0);   // Move `vec0` into `fill_vec`, returning `vec1`.

        assert_eq!(vec1, vec![22, 44, 66, 88]); // Verify that 88 was added.
    }
}


