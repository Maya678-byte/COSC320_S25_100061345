// Added `pub` to make the struct accessible outside this module.
// Without `pub`, tests in the `tests` module cannot use this struct.
pub struct ColorRegularStruct {
    pub red: u8,   // `pub` makes fields accessible outside this struct.
    pub green: u8, // Needed for tests to read these values.
    pub blue: u8,
}

// Similarly, made the tuple struct `pub` to allow access in tests.
pub struct ColorTupleStruct(pub u8, pub u8, pub u8);

// Used `#[derive(Debug)]` so that `UnitStruct` can be printed using `{:?}` in tests.
#[derive(Debug)]
pub struct UnitStruct;

#[allow(dead_code)] // Prevent warnings about unused struct and methods
fn main() {
    // You can optionally experiment here.
}

#[cfg(test)]
mod tests {
    use super::*; // Imports everything from the outer scope to be used inside tests.

    #[test]
    fn regular_structs() {
        // Created an instance of `ColorRegularStruct` with RGB values for green.
        let green = ColorRegularStruct { red: 0, green: 255, blue: 0 };

        // Verified that the struct fields hold expected values.
        assert_eq!(green.red, 0);
        assert_eq!(green.green, 255);
        assert_eq!(green.blue, 0);
    }

    #[test]
    fn tuple_structs() {
        // Created an instance of `ColorTupleStruct` representing green.
        let green = ColorTupleStruct(0, 255, 0);

        // Checked if the values match the expected RGB values.
        assert_eq!(green.0, 0);
        assert_eq!(green.1, 255);
        assert_eq!(green.2, 0);
    }

    #[test]
    fn unit_structs() {
        // Instantiated `UnitStruct` to test its debug output.
        let unit_struct = UnitStruct;

        // Verified that `format!("{unit_struct:?}")` correctly prints "UnitStructs are fun!"
        let message = format!("{unit_struct:?}s are fun!");
        assert_eq!(message, "UnitStructs are fun!");
    }
}
