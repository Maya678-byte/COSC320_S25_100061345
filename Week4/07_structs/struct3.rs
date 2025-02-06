#[derive(Debug)]
#[allow(dead_code)] // Prevent warnings about unused struct and methods
struct Package {
    sender_country: String,
    recipient_country: String,
    weight_in_grams: u32,
}

impl Package {
#[allow(dead_code)] // Prevent warnings about unused struct and methods
    fn new(sender_country: String, recipient_country: String, weight_in_grams: u32) -> Self {
        if weight_in_grams < 10 {
            // This isn't how you should handle errors in Rust, but we will
            // learn about error handling later.
            panic!("Can't ship a package with weight below 10 grams");
        }

        Self {
            sender_country,
            recipient_country,
            weight_in_grams,
        }
    }

    // Determine if the package is international based on sender and recipient countries.
#[allow(dead_code)] // Prevent warnings about unused struct and methods
    fn is_international(&self) -> bool {
        self.sender_country != self.recipient_country
    }

    // Calculate the transport fees based on the weight of the package.
#[allow(dead_code)] // Prevent warnings about unused struct and methods
    fn get_fees(&self, cents_per_gram: u32) -> u32 {
        self.weight_in_grams * cents_per_gram
    }
}

#[allow(dead_code)] // Prevent warnings about unused struct and methods
fn main() {
    // You can optionally experiment here.
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    #[should_panic]
    fn fail_creating_weightless_package() {
        let sender_country = String::from("Spain");
        let recipient_country = String::from("Austria");

        // This will panic because the package weight is below 10 grams.
        Package::new(sender_country, recipient_country, 5);
    }

    #[test]
    fn create_international_package() {
        let sender_country = String::from("Spain");
        let recipient_country = String::from("Russia");

        let package = Package::new(sender_country, recipient_country, 1200);

        // Since the sender and recipient countries are different, it should be international.
        assert!(package.is_international());
    }

    #[test]
    fn create_local_package() {
        let sender_country = String::from("Canada");
        let recipient_country = sender_country.clone();

        let package = Package::new(sender_country, recipient_country, 1200);

        // Since the sender and recipient countries are the same, it should be local (not international).
        assert!(!package.is_international());
    }

    #[test]
    fn calculate_transport_fees() {
        let sender_country = String::from("Spain");
        let recipient_country = String::from("Spain");

        let cents_per_gram = 3;

        let package = Package::new(sender_country, recipient_country, 1500);

        // Calculating the transport fees based on weight and rate (cents per gram).
        assert_eq!(package.get_fees(cents_per_gram), 4500);
        assert_eq!(package.get_fees(cents_per_gram * 2), 9000);
    }
}
