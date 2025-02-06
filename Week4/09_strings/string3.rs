fn trim_me(input: &str) -> &str {
    input.trim() // Removes whitespace from both ends
}

fn compose_me(input: &str) -> String {
    format!("{} world!", input) // Appends " world!" to the input string
}

fn replace_me(input: &str) -> String {
    input.replace("cars", "balloons") // Replaces "cars" with "balloons"
}

fn main() {
    // Example usage to ensure functions are used
    let trimmed = trim_me("   Hello!   ");
    println!("Trimmed: {}", trimmed);

    let composed = compose_me("Hello");
    println!("Composed: {}", composed);

    let replaced = replace_me("I think cars are cool");
    println!("Replaced: {}", replaced);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn trim_a_string() {
        assert_eq!(trim_me("Hello!     "), "Hello!");
        assert_eq!(trim_me("  What's up!"), "What's up!");
        assert_eq!(trim_me("   Hola!  "), "Hola!");
    }

    #[test]
    fn compose_a_string() {
        assert_eq!(compose_me("Hello"), "Hello world!");
        assert_eq!(compose_me("Goodbye"), "Goodbye world!");
    }

    #[test]
    fn replace_a_string() {
        assert_eq!(
            replace_me("I think cars are cool"),
            "I think balloons are cool",
        );
        assert_eq!(
            replace_me("I love to look at cars"),
            "I love to look at balloons",
        );
    }
}
