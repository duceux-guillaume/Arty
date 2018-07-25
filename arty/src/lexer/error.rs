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
        return State::Acc
    }

    fn reset(&mut self) {
        self.token.clear();
        self.state = State::Sta;
    }

    fn token(&mut self) -> Token {
        return Token::Error(self.token.clone())
    }
}

pub struct None {
    state: State
}
impl None {
    pub fn new() -> Self {
        return None { state: State::Acc }
    }
}
impl ILexer for None {
    fn eat(&mut self, c: char) -> State {
        let new_state = match self.state {
            State::Sta => {
                if helper::is_blank(c) {
                    State::Ong
                } else {
                    State::Rej
                }
            },
            State::Ong => {
                if helper::is_blank(c) {
                    State::Ong
                } else {
                    State::Acc
                }
            },
            _ => self.state,
        };
        self.state = new_state;
        return self.state
    }

    fn reset(&mut self) {
        self.state = State::Sta;
    }

    fn token(&mut self) -> Token {
        return Token::None
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


