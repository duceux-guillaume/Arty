use language::token::Token;

pub trait ILexer {
    fn eat(&mut self, c: char) -> State;
    fn reset(&mut self);
    fn token(&mut self) -> Token;
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum State {
    Acc,
    Ong,
    Rej,
    Sta,
}
