#[allow(dead_code)] fn fill_vec(mut vec: Vec<i32>) -> Vec<i32> { // Fix: Suppressed warning and made `vec` mutable
    vec.push(88); // Now `vec` can be modified
    vec // Return the modified vector
}

fn main() { let _ = fill_vec(vec![22, 44, 66]); } // Fix: Ensured function is used to remove warning

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn move_semantics3() {
        let vec0 = vec![22, 44, 66]; // Original vector
        let vec1 = fill_vec(vec0); // Pass ownership to `fill_vec`

        assert_eq!(vec1, [22, 44, 66, 88]); // Test passes if 88 is correctly added
    }
}
