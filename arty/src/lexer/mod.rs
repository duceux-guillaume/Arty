mod helper;
pub mod interface;
mod number;
mod error;
mod identifier;
mod ctrlop;
mod mathop;

use self::number::Number;
use self::ctrlop::CtrlOp;
use self::mathop::MathOp;
use self::identifier::Identifier;
use self::error::Error;
use self::interface::State;

use language::Token;

pub struct Lexer {
    automatas: Vec<Box<interface::ILexer>>,
    data: String,
    tokens: Vec<Token>,
    pos: usize,
}

impl Lexer {
    pub fn new(data: String) -> Self {
        let mut lexer = Lexer {
            automatas: Vec::new(),
            data,
            tokens: Vec::new(),
            pos: 0,
        };
        lexer.automatas.push(Box::new(Number::new()));
        lexer.automatas.push(Box::new(Identifier::new()));
        lexer.automatas.push(Box::new(CtrlOp::new()));
        lexer.automatas.push(Box::new(MathOp::new()));
        let data = lexer.data.clone();
        lexer.tokens = lexer.process(data); //TODO(Guillaume): make a lazy evaluation
        return lexer
    }

    fn process(&mut self, string: String) -> Vec<Token> {
        let mut result = Vec::new();
        let char_vec:Vec<char> = string.chars().collect();
        let mut pos = 0;
        while pos < char_vec.len() {
            let c = char_vec[pos];
            let mut opt_token = None;
            let mut ong_count = 0;
            for l in self.automatas.iter_mut() {
                match l.eat(c) {
                    State::Acc => opt_token = Some(l.token()),
                    State::Ong => ong_count += 1,
                    _ => {},
                }
            }
            // If we got an accepted on this char, we need to parse it again after global reset
            if ong_count == 0 {
                match opt_token {
                    Some(t) => {
                        println!("found token: {}", t);
                        result.push(t);
                    },
                    None => {
                        println!("found token: {}", Token::Eof);
                        result.push(Token::Eof);
                        pos += 1;
                    },
                }
                for l in self.automatas.iter_mut() {
                    l.reset()
                }
            } else {
                pos += 1;
            }
        }
        result.push(Token::Eof);
        return result;
    }

    pub fn get(&self, idx: usize) -> Token {
        if idx >= self.tokens.len() {
            return Token::Eof
        } else {
            return self.tokens[idx].clone()
        }
    }

    pub fn next(&mut self) -> Token {
        let token = self.get(self.pos);
        self.pos += 1;
        return token;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn integer() {
        let string = String::from("123 ");
        let mut lexer = Lexer::new();
        let res = lexer.process(string.clone());
        assert_eq!(1, res.len());
        assert_eq!(string.trim(), Token::from(res[0].clone()));
    }
}


