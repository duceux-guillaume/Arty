use lexer::interface::ILexer;
use lexer::interface::State;
use language::Token;
use lexer::helper;

pub struct MathOp {
    token: Token,
    state: State,
}

impl MathOp {
    pub fn new() -> Self {
        return MathOp {
            token: Token::Eof,
            state: State::Sta,
        }
    }
}

impl ILexer for MathOp {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                match c {
                    '+' => self.token = Token::Plus,
                    '-' => self.token = Token::Minus,
                    '*' => self.token = Token::Times,
                    '/' => self.token = Token::Divide,
                    '%' => self.token = Token::Modulo,
                    _ => {},
                }
                if self.token == Token::Eof {
                    State::Rej
                } else {
                    State::Ong
                }
            },
            _ => {
                if helper::is_blanck(c) || helper::is_digit(c) {
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
        self.token = Token::Eof;
        self.state = State::Sta;
    }

    fn token(&mut self) -> Token {
        return self.token.clone()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn operator() {
        let string = String::from("++");
        let mut number = MathOp::new();
        for c in string.chars() {
            number.eat(c);
        }
        assert_eq!(State::Rej, number.eat(' '));
    }

    #[test]
    fn math_op() {
        let string = String::from("+");
        let mut number = MathOp::new();
        for c in string.chars() {
            assert_eq!(State::Ong, number.eat(c));
        }
        assert_eq!(State::Acc, number.eat(' '));
    }
}
