#[derive(Debug)]
#[allow(dead_code)] // Prevent warnings about unused struct
struct Order {
    name: String,
    year: u32,
    made_by_phone: bool,
    made_by_mobile: bool,
    made_by_email: bool,
    item_number: u32,
    count: u32,
}

// Function to create a template order with default values.
#[allow(dead_code)] // Prevent warnings about unused struct
fn create_order_template() -> Order {
    Order {
        name: String::from("Bob"), // Default name
        year: 2019, // Default year
        made_by_phone: false, // Order was not made by phone
        made_by_mobile: false, // Order was not made by mobile
        made_by_email: true, // Order was made by email
        item_number: 123, // Default item number
        count: 0, // Default count (no items ordered yet)
    }
}

#[allow(dead_code)] // Prevent warnings about unused struct and methods
fn main() {
    // You can optionally experiment here.
}

#[cfg(test)]
mod tests {
    use super::*; // Import everything from the main module for testing.

    #[test]
    fn your_order() {
        let order_template = create_order_template(); // Get a template order.

        // Creating a new order by modifying only necessary fields using update syntax.
        let your_order = Order {
            name: String::from("Hacker in Rust"), // Updating the name
            count: 1, // Updating count to 1
            ..order_template // Copying remaining fields from order_template
        };

        // Checking if the order was correctly updated.
        assert_eq!(your_order.name, "Hacker in Rust"); // Name must be updated
        assert_eq!(your_order.year, order_template.year); // Year should be the same as the template
        assert_eq!(your_order.made_by_phone, order_template.made_by_phone); // Must match template
        assert_eq!(your_order.made_by_mobile, order_template.made_by_mobile); // Must match template
        assert_eq!(your_order.made_by_email, order_template.made_by_email); // Must match template
        assert_eq!(your_order.item_number, order_template.item_number); // Must match template
        assert_eq!(your_order.count, 1); // Count should be updated to 1
    }
}
