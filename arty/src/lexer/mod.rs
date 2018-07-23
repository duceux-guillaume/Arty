mod helper;
mod interface;
mod number;
mod error;

use self::number::Number;
use self::error::Error;
use self::interface::Token;
use self::interface::State;
use self::interface::Type;

pub struct Lexer {
    automatas: Vec<Box<interface::ILexer>>,
}

impl Lexer {
    pub fn new() -> Self {
        let mut lexer = Lexer {
            automatas: Vec::new()
        };
        lexer.automatas.push(Box::new(Error::new()));
        lexer.automatas.push(Box::new(Number::new()));
        return lexer
    }

    pub fn process(&mut self, string: String) -> Vec<Token> {
        let mut result = Vec::new();
        for c in string.chars() {
            let mut opt_token = None;
            let mut ong_count = 0;
            for l in self.automatas.iter_mut() {
                match l.eat(c) {
                    State::ACC => opt_token = Some(l.token()),
                    State::ONG => ong_count += 1,
                    _ => {},
                }
            }
            if ong_count == 0 {
                match opt_token {
                    Some(t) => result.push(t),
                    None => println!("No token found"),
                }
                for l in self.automatas.iter_mut() {
                    l.reset()
                }
            }
        }

        return result;
    }
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn integer() {
        let string = String::from("123 ");
        let mut lexer = Lexer::new();
        let res = lexer.process(string.clone());
        assert_eq!(1, res.len());
        assert_eq!(Type::NUMBER, res[0].get_type());
        assert_eq!(string.trim(), res[0].get_val());
    }
    
    #[test]
    fn error() {
        let string = String::from("q5# ");
        let mut lexer = Lexer::new();
        let res = lexer.process(string.clone());
        assert_eq!(1, res.len());
        assert_eq!(Type::ERROR, res[0].get_type());
        assert_eq!(string.trim(), res[0].get_val());
    }
}


