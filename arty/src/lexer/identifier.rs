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
            state: State::Sta,
        }
    }
}

impl ILexer for Identifier {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                if helper::is_letter(c) {
                    self.token.push(c);
                    State::Ong
                } else {
                    State::Rej
                }
            },
            _ => {
                if helper::is_digit(c) || helper::is_letter(c) {
                    self.token.push(c);
                    State::Ong
                } else if helper::is_blanck(c) {
                    State::Acc
                } else {
                    State::Rej
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
        return Token::Identifier(self.token.clone())
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
            assert_eq!(State::Rej, lexer.eat(c));
        }
        assert_eq!(State::Rej, lexer.eat(' '));
    }

    #[test]
    fn identifier() {
        let string = String::from("a1");
        let mut lexer = Identifier::new();
        for c in string.chars() {
            assert_eq!(State::Ong, lexer.eat(c));
        }
        assert_eq!(State::Acc, lexer.eat(' '));
    }

    #[test]
    fn identifier2() {
        let string = String::from("aname");
        let mut lexer = Identifier::new();
        for c in string.chars() {
            assert_eq!(State::Ong, lexer.eat(c));
        }
        assert_eq!(State::Acc, lexer.eat(' '));
    }

    #[test]
    fn blanck() {
        let string = String::from(" ");
        let mut lexer = Identifier::new();
        for c in string.chars() {
            assert_eq!(State::Rej, lexer.eat(c));
        }
        assert_eq!(State::Rej, lexer.eat(' '));
    }
}
