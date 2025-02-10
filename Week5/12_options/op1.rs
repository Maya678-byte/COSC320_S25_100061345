fn maybe_icecream(hour_of_day: u16) -> Option<u16> {
    if hour_of_day > 23 {
        None // Invalid hour
    } else if hour_of_day >= 22 {
        Some(0) // No ice cream left after 10 PM
    } else {
        Some(5) // 5 scoops available before 10 PM
    }
}

fn main() {
    // You can optionally experiment here.
    print!("{:?}", maybe_icecream(12));
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn raw_value() {
// TODO: Fix this test. How do you get the value contained in the
        // Option?
        // Test the Option value returned by maybe_icecream
        let icecreams = maybe_icecream(12);

	match icecreams {
            Some(scoops) => assert_eq!(scoops, 5), // Check if scoops is 5
            None => panic!("Expected Some, but got None"), // Panic if None is returned
        }

        assert_eq!(icecreams, Some(5)); // Fix: Check for Option wrapping
	print!("FROM TEST{:?}", maybe_icecream(12));


    }

    #[test]
    fn check_icecream() {
        assert_eq!(maybe_icecream(0), Some(5));
        assert_eq!(maybe_icecream(9), Some(5));
        assert_eq!(maybe_icecream(18), Some(5));
        assert_eq!(maybe_icecream(22), Some(0));
        assert_eq!(maybe_icecream(23), Some(0));
        assert_eq!(maybe_icecream(24), None);
        assert_eq!(maybe_icecream(25), None);
    }
}
