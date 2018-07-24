use lexer::interface::ILexer;
use language::Token;
use lexer::interface::State;
use lexer::helper;


pub struct Error {
    token: String,
    state: State,
}

impl Error {
    pub fn new () -> Self {
        return Error {
            token: String::new(),
            state: State::Sta,
        }
    }
}

impl ILexer for Error {
    fn eat(&mut self, c: char) -> State {
        let new_state = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            _ => {
                if helper::is_blanck(c) {
                    State::Acc
                } else {
                    self.token.push(c);
                    State::Ong
                }
            }
        };
        self.state = new_state;
        return self.state
    }

    fn reset(&mut self) {
        self.token.clear();
        self.state = State::Sta;
    }

    fn token(&mut self) -> Token {
        return Token::Error(self.token.clone())
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
            assert_eq!(State::Ong, lexer.eat(c));
        }
        assert_eq!(State::Acc, lexer.eat(' '));
    }

    #[test]
    fn identifier() {
        let string = String::from("a1");
        let mut lexer = Error::new();
        for c in string.chars() {
            assert_eq!(State::Ong, lexer.eat(c));
        }
        assert_eq!(State::Acc, lexer.eat(' '));
    }
}


