use lexer::interface::ILexer;
use lexer::interface::State;
use language::Token;
use lexer::helper;

pub struct CtrlOp {
    token: String,
    state: State,
}

impl CtrlOp {
    pub fn new() -> Self {
        return CtrlOp {
            token: String::new(),
            state: State::STA,
        }
    }
}

impl ILexer for CtrlOp {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::REJ => self.state,
            State::ACC => self.state,
            State::STA => {
                if helper::is_ctrl_operator(c) {
                    self.token.push(c);
                    State::ONG
                } else {
                    State::REJ
                }
            },
            _ => {
                if helper::is_blanck(c) {
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
        return Token::ERROR(self.token.clone())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ctrl_operator() {
        let string = String::from("|");
        let mut lexer = CtrlOp::new();
        for c in string.chars() {
            assert_eq!(State::ONG, lexer.eat(c));
        }
        assert_eq!(State::ACC, lexer.eat(' '));
    }

    #[test]
    fn blanck() {
        let string = String::from(" ");
        let mut lexer = CtrlOp::new();
        for c in string.chars() {
            assert_eq!(State::REJ, lexer.eat(c));
        }
        assert_eq!(State::REJ, lexer.eat(' '));
    }
}
