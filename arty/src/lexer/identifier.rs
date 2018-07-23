use lexer::interface::ILexer;
use lexer::interface::State;
use language::Token;
use lexer::helper;

pub struct Identifier {
    token: String,
    state: State,
}

impl Identifier {
    pub fn new() -> Self {
        return Identifier {
            token: String::new(),
            state: State::STA,
        }
    }
}

impl ILexer for Identifier {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::REJ => self.state,
            State::ACC => self.state,
            State::STA => {
                if helper::is_letter(c) {
                    self.token.push(c);
                    State::ONG
                } else {
                    State::REJ
                }
            },
            _ => {
                if helper::is_digit(c) || helper::is_letter(c) {
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
        return Token::IDENTIFIER(self.token.clone())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn integer() {
        let string = String::from("1234567890");
        let mut lexer = Identifier::new();
        for c in string.chars() {
            assert_eq!(State::REJ, lexer.eat(c));
        }
        assert_eq!(State::REJ, lexer.eat(' '));
    }

    #[test]
    fn identifier() {
        let string = String::from("a1");
        let mut lexer = Identifier::new();
        for c in string.chars() {
            assert_eq!(State::ONG, lexer.eat(c));
        }
        assert_eq!(State::ACC, lexer.eat(' '));
    }

    #[test]
    fn identifier2() {
        let string = String::from("aname");
        let mut lexer = Identifier::new();
        for c in string.chars() {
            assert_eq!(State::ONG, lexer.eat(c));
        }
        assert_eq!(State::ACC, lexer.eat(' '));
    }

    #[test]
    fn blanck() {
        let string = String::from(" ");
        let mut lexer = Identifier::new();
        for c in string.chars() {
            assert_eq!(State::REJ, lexer.eat(c));
        }
        assert_eq!(State::REJ, lexer.eat(' '));
    }
}
