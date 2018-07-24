   
pub fn is_digit(c: char) -> bool {
    return c >= '0' && c <= '9'
}

pub fn is_blanck(c: char) -> bool {
    return c == ' ' || c == '\n' || c == '\t'
}

pub fn is_letter(c: char) -> bool {
    return (c >= 'a' && c <= 'z') || 
        (c >= 'A' && c <= 'Z')
}

pub fn is_ctrlop(c: char) -> bool {
    return c == '|' ||
        c == ';' ||
        c == '&' ||
        c == '>';
}

pub fn is_mathop(c: char) -> bool {
    return c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '%' ||
        c == '/';
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn digits() {
        let digits = String::from("0123456789");
        for c in digits.chars() {
            assert_eq!(true, is_digit(c));
            assert_eq!(false, is_blanck(c));
            assert_eq!(false, is_letter(c));
            assert_eq!(false, is_ctrlop(c));
        }
    }

    #[test]
    fn letters() {
        let letters = String::from("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
        for c in letters.chars() {
            assert_eq!(false, is_digit(c));
            assert_eq!(false, is_blanck(c));
            assert_eq!(true, is_letter(c));
            assert_eq!(false, is_ctrlop(c));
        }
    }
    
    #[test]
    fn blancks() {
        let blancks = String::from(" \n\t");
        for c in blancks.chars() {
            assert_eq!(false, is_digit(c));
            assert_eq!(true, is_blanck(c));
            assert_eq!(false, is_letter(c));
            assert_eq!(false, is_ctrlop(c));
        }
    }


    #[test]
    fn crtl_operators() {
        let blancks = String::from("&|;>");
        for c in blancks.chars() {
            assert_eq!(false, is_digit(c));
            assert_eq!(false, is_blanck(c));
            assert_eq!(false, is_letter(c));
            assert_eq!(true, is_ctrlop(c));
        }
    }
}
    
