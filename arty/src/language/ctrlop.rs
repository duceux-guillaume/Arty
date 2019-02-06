use  language::interface::ILexer;
use  language::interface::State;
use language::token::Token;
use language::helper;

pub struct CtrlOp {
    token: String,
    state: State,
}

impl CtrlOp {
    pub fn new() -> Self {
        return CtrlOp {
            token: String::new(),
            state: State::Sta,
        }
    }
}

impl ILexer for CtrlOp {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                if helper::is_ctrlop(c) {
                    self.token.push(c);
                    State::Ong
                } else {
                    State::Rej
                }
            },
            _ => {
                if helper::is_blank(c) {
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
        return Token::Error(self.token.clone())
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
            assert_eq!(State::Ong, lexer.eat(c));
        }
        assert_eq!(State::Acc, lexer.eat(' '));
    }

    #[test]
    fn blanck() {
        let string = String::from(" ");
        let mut lexer = CtrlOp::new();
        for c in string.chars() {
            assert_eq!(State::Rej, lexer.eat(c));
        }
        assert_eq!(State::Rej, lexer.eat(' '));
    }
}
