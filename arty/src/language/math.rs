use  language::interface::ILexer;
use  language::interface::State;
use language::token::Token;
use language::helper;

pub struct MathOp {
    val: char,
    token: Token,
    state: State,
}

impl MathOp {
    pub fn new() -> Self {
        return MathOp {
            val: ' ',
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
                    '(' => self.token = Token::ParO,
                    ')' => self.token = Token::ParC,
                    '|' => self.token = Token::Pipe,
                    '<' => self.token = Token::CheO,
                    '>' => self.token = Token::CheC,
                    _ => {},
                }
                if self.token == Token::Eof {
                    State::Rej
                } else {
                    State::Ong
                }
            },
            State::Ong => {
                if helper::is_blank(c) || helper::is_digit(c) {
                    State::Acc
                }

                else if helper::is_mathop(self.val) {
                    if c == '(' {
                        State::Acc
                    } else {
                        State::Rej
                    }
                }

                else if self.val == ')' {
                    if helper::is_mathop(c) {
                        State::Acc
                    } else {
                        State::Rej
                    }
                }

                else {
                    State::Rej
                }
            },
        };
        self.val = c;
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
            State::Ong => {
                if helper::is_digit(c) {
                    self.token.push(c);
                    State::Ong
                } else if c == '.' {
                    self.token.push(c);
                    State::Ong
                } else if helper::is_close(c) || helper::is_blank(c) || helper::is_mathop(c) ||
                        helper::is_ctrlop(c) || helper::is_open(c) {
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
    fn blank() {
        let string = String::from(" ");
        let mut number = Number::new();
        for c in string.chars() {
            assert_eq!(State::Rej, number.eat(c));
        }
        assert_eq!(State::Rej, number.eat(' '));
    }

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
