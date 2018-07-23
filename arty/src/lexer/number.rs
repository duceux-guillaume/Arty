use lexer::interface::ILexer;
use lexer::interface::State;
use language::Token;
use lexer::helper;

pub struct Number {
    token: String,
    state: State,
}

impl Number {
    pub fn new() -> Self {
        return Number {
            token: String::new(),
            state: State::STA,
        }
    }
}

impl ILexer for Number {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::REJ => self.state,
            State::ACC => self.state,
            State::STA => {
                if helper::is_digit(c) {
                    self.token.push(c);
                    State::ONG
                } else {
                    State::REJ
                }
            },
            _ => {
                if helper::is_digit(c) {
                    self.token.push(c);
                    State::ONG
                } else if helper::is_blanck(c) {
                    State::ACC
                } else {
                    State::REJ
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
        return Token::NUMBER(self.token.clone())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn integer() {
        let string = String::from("1234567890");
        let mut number = Number::new();
        for c in string.chars() {
            assert_eq!(State::ONG, number.eat(c));
        }
        assert_eq!(State::ACC, number.eat(' '));
    }

    #[test]
    fn identifier() {
        let string = String::from("a1");
        let mut number = Number::new();
        for c in string.chars() {
            assert_eq!(State::REJ, number.eat(c));
        }
        assert_eq!(State::REJ, number.eat(' '));
    }
    
    #[test]
    fn blamck() {
        let string = String::from(" ");
        let mut number = Number::new();
        for c in string.chars() {
            assert_eq!(State::REJ, number.eat(c));
        }
        assert_eq!(State::REJ, number.eat(' '));
    }
}
