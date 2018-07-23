use lexer::interface::ILexer;
use lexer::interface::Token;
use lexer::interface::State;
use lexer::interface::Type;
use lexer::helper;


pub struct Error {
    token: String,
    state: State,
}

impl Error {
    pub fn new () -> Self {
        return Error {
            token: String::new(),
            state: State::STA,
        }
    }
}

impl ILexer for Error {
    fn eat(&mut self, c: char) -> State {
        let new_state = match self.state {
            State::REJ => self.state,
            State::ACC => self.state,
            _ => {
                if helper::is_blanck(c) {
                    State::ACC
                } else {
                    self.token.push(c);
                    State::ONG
                }
            }
        };
        self.state = new_state;
        return self.state
    }

    fn reset(&mut self) {
        self.token.clear();
        self.state = State::STA;
    }

    fn token(&mut self) -> Token {
        return Token::new(Type::ERROR, self.token.clone())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn integer() {
        let string = String::from("1234567890");
        let mut lexer = Error::new();
        for c in string.chars() {
            assert_eq!(State::ONG, lexer.eat(c));
        }
        assert_eq!(State::ACC, lexer.eat(' '));
    }

    #[test]
    fn identifier() {
        let string = String::from("a1");
        let mut lexer = Error::new();
        for c in string.chars() {
            assert_eq!(State::ONG, lexer.eat(c));
        }
        assert_eq!(State::ACC, lexer.eat(' '));
    }
}


