pub fn is_digit(c: char) -> bool {
    return c >= '0' && c <= '9';
}

pub fn is_blank(c: char) -> bool {
    return c == ' ' || c == '\n' || c == '\t';
}

pub fn is_ctrlop(c: char) -> bool {
    return c == '|' || c == ';' || c == '&' || c == '>';
}

pub fn is_mathop(c: char) -> bool {
    return c == '+' || c == '-' || c == '*' || c == '%' || c == '/';
}

pub fn is_close(c: char) -> bool {
    return c == ')' || c == '}' || c == ']';
}

pub fn is_open(c: char) -> bool {
    return c == '(' || c == '{' || c == '[';
}

pub fn is_path_char(c: char) -> bool {
    return !c.is_whitespace();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn digits() {
        let digits = String::from("0123456789");
        for c in digits.chars() {
            assert_eq!(true, is_digit(c));
            assert_eq!(false, is_blank(c));
            assert_eq!(false, is_ctrlop(c));
        }
    }

    #[test]
    fn letters() {
        let letters = String::from("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
        for c in letters.chars() {
            assert_eq!(false, is_digit(c));
            assert_eq!(false, is_blank(c));
            assert_eq!(false, is_ctrlop(c));
        }
    }

    #[test]
    fn blancks() {
        let blancks = String::from(" \n\t");
        for c in blancks.chars() {
            assert_eq!(false, is_digit(c));
            assert_eq!(true, is_blank(c));
            assert_eq!(false, is_ctrlop(c));
        }
    }

    #[test]
    fn crtl_operators() {
        let blancks = String::from("&|;>");
        for c in blancks.chars() {
            assert_eq!(false, is_digit(c));
            assert_eq!(false, is_blank(c));
            assert_eq!(true, is_ctrlop(c));
        }
    }
}
