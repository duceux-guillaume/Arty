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
            state: State::Sta,
        }
    }
}

impl ILexer for Number {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                if helper::is_digit(c) {
                    self.token.push(c);
                    State::Ong
                } else {
                    State::Rej
                }
            },
            _ => {
                if helper::is_digit(c) {
                    self.token.push(c);
                    State::Ong
                } else if helper::is_blanck(c) || helper::is_mathop(c) || helper::is_ctrlop(c) {
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
        return Token::Number(self.token.clone())
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
            assert_eq!(State::Ong, number.eat(c));
        }
        assert_eq!(State::Acc, number.eat(' '));
    }

    #[test]
    fn identifier() {
        let string = String::from("a1");
        let mut number = Number::new();
        for c in string.chars() {
            assert_eq!(State::Rej, number.eat(c));
        }
        assert_eq!(State::Rej, number.eat(' '));
    }
    
    #[test]
    fn blamck() {
        let string = String::from(" ");
        let mut number = Number::new();
        for c in string.chars() {
            assert_eq!(State::Rej, number.eat(c));
        }
        assert_eq!(State::Rej, number.eat(' '));
    }
}
