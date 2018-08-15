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
use self::name::Opts;
use self::interface::State;

use language::Token;

use std::result;
use std::error::Error;
use std::fmt;
use lexer::name::ChangeDir;
use lexer::name::Path;

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
impl Error for LexicalError {
    fn description(&self) -> &str {
        return "lexical error"
    }
}


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
        lexer.automatas.push(Box::new(Number::new()));
        lexer.automatas.push(Box::new(ChangeDir::new()));
        lexer.automatas.push(Box::new(Path::new()));
        lexer.automatas.push(Box::new(Empty::new()));
        lexer.automatas.push(Box::new(Cmd::new()));
        lexer.automatas.push(Box::new(CtrlOp::new()));
        lexer.automatas.push(Box::new(MathOp::new()));
        lexer.automatas.push(Box::new(Opts::new()));
        return Ok(lexer)
    }

    fn process(&mut self) -> Result<Token> {
        let char_vec:Vec<char> = self.data.chars().collect();
        while self.pos < char_vec.len() {
            let c = char_vec[self.pos];
            let mut acc_tokens: Vec<Token> = Vec::new();
            let mut ong_count = 0;
            for l in self.automatas.iter_mut() {
                match l.eat(c) {
                    State::Acc => acc_tokens.push(l.token()),
                    State::Ong => ong_count += 1,
                    _ => {},
                }
            }
            // If we got an accepted on this char, we need to parse it again after global reset
            if ong_count == 0 {
                for l in self.automatas.iter_mut() {
                    l.reset()
                }
                match self.select_token(acc_tokens) {
                    Some(t) => {
                        match t.clone() {
                            Token::None => {},
                            _ => {
                                self.tokens.push(t.clone());
                                return Ok(t)
                            },
                        }
                    },
                    None => return Err(Box::new(
                        LexicalError::new(format!("No token found at: {}", c)))),
                }
            } else {
                self.pos += 1;
            }
        }
        self.tokens.push(Token::Eof);
        return Ok(Token::Eof);
    }

    fn select_token(&self, candidates: Vec<Token>) -> Option<Token> {
        if candidates.len() == 0 {
            return None
        }
        if candidates.len() == 1 {
            return Some(candidates.first().unwrap().clone())
        }
        // Look for a change directory or a cmd
        if self.tokens.is_empty() {
            for token in candidates.iter() {
                if token.description() == "ChangeDir" {
                    return Some(token.clone())
                }
            }
            for token in candidates.iter() {
                if token.description() == "Cmd" {
                    return Some(token.clone())
                }
            }
        } else {
            for token in candidates.iter() {
                if token.description() != "Cmd" && token.description() != "ChangeDir" {
                    return Some(token.clone())
                }
            }
        }
        return Some(candidates.first().unwrap().clone())
    }

    pub fn get(&self, idx: usize) -> Token {
        if idx >= self.tokens.len() {
            return Token::Eof
        } else {
            return self.tokens[idx].clone()
        }
    }

    pub fn next(&mut self) -> Result<Token> {
        let token = self.process()?;
        return Ok(token);
    }
}
