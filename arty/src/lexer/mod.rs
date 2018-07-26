mod helper;
pub mod interface;
mod name;
mod ctrlop;
mod math;

use self::math::Number;
use self::ctrlop::CtrlOp;
use self::math::MathOp;
use self::name::Empty;
use self::name::Cmd;
use self::interface::State;

use language::Token;

use std::result;
use std::error::Error;
use std::fmt;

#[derive(Debug)]
struct LexicalError {
    cause: String
}
impl LexicalError {
    fn new(cause: String) -> Self {
        return LexicalError {
            cause,
        }
    }
}
impl Error for LexicalError {}
impl fmt::Display for LexicalError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result { write!(f, "error: {}", self.cause) }
}

type Result<T> = result::Result<T, Box<Error>>;

pub struct Lexer {
    automatas: Vec<Box<interface::ILexer>>,
    data: String,
    tokens: Vec<Token>,
    pos: usize,
}

impl Lexer {
    pub fn new(data: String) -> Result<Self> {
        let mut lexer = Lexer {
            automatas: Vec::new(),
            data,
            tokens: Vec::new(),
            pos: 0,
        };
        lexer.automatas.push(Box::new(Empty::new()));
        lexer.automatas.push(Box::new(Number::new()));
        lexer.automatas.push(Box::new(Cmd::new()));
        lexer.automatas.push(Box::new(CtrlOp::new()));
        lexer.automatas.push(Box::new(MathOp::new()));
        let data = lexer.data.clone();
        lexer.tokens = lexer.process(data)?; //TODO(Guillaume): make a lazy evaluation
        return Ok(lexer)
    }

    fn process(&mut self, string: String) -> Result<Vec<Token>> {
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
                        match t {
                            Token::None => {},
                            _ => result.push(t),
                        }
                    },
                    None => return Err(Box::new(
                        LexicalError::new(String::from("No token found")))),
                }
                for l in self.automatas.iter_mut() {
                    l.reset()
                }
            } else {
                pos += 1;
            }
        }
        result.push(Token::Eof);
        return Ok(result);
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
