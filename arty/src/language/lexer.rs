use language::math::Number;
use language::ctrlop::CtrlOp;
use language::math::MathOp;
use language::name::Empty;
use language::name::Cmd;
use language::name::CmdArgs;
use language::interface::State;
use language::interface;
use language::token::Token;

use std::result;
use std::error::Error;
use std::fmt;
use language::name::ChangeDir;
use language::name::Path;
use language::name::StringLexer;
use filesystem::SearchFor;
use std::env;

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
    scanner_pos: usize,
    last_token_pos: usize,
}

impl Lexer {
    pub fn new(data: String) -> Self {
        return Lexer {
            automatas: Vec::new(),
            data,
            tokens: Vec::new(),
            scanner_pos: 0,
            last_token_pos: 0,
        }
    }

    fn process(&mut self) -> Result<Token> {
        self.automatas = self.select_lexer();
        let mut char_vec:Vec<char> = self.data.chars().collect();
        if char_vec.len() > 0 && char_vec.last().unwrap() != &'\n' {
            char_vec.push('\n');
        }
        while self.scanner_pos < char_vec.len() {
            let c = char_vec[self.scanner_pos];
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
                self.automatas = self.select_lexer();
                match self.select_token(acc_tokens) {
                    Some(t) => {
                        match t.clone() {
                            Token::None => {
                                self.scanner_pos += 1;
                                self.last_token_pos += 1;
                            },
                            _ => {
                                self.tokens.push(t.clone());
                                self.scanner_pos = self.last_token_pos + t.len();
                                self.last_token_pos = self.scanner_pos;
                                return Ok(t)
                            },
                        }
                    },
                    None => return Err(Box::new(
                        LexicalError::new(format!("No token found at: {}", c)))),
                }
            } else {
                self.scanner_pos += 1;
            }
        }
        self.tokens.push(Token::Eof);
        return Ok(Token::Eof);
    }

    fn select_lexer(&self) -> Vec<Box<interface::ILexer>> {
        if self.tokens.is_empty() {
            return vec![
                Box::new(Number::new()),
                Box::new(ChangeDir::new()),
                Box::new(Path::new()),
                Box::new(Cmd::new()),
                Box::new(MathOp::new()),
                Box::new(Empty::new()),
            ]
        }
        if let &Token::Cmd(ref _cmd) = self.tokens.first().unwrap() {
            return vec![
                Box::new(MathOp::new()),
                Box::new(CmdArgs::new()),
                Box::new(Empty::new())
            ]
        }
        if let &Token::ChangeDir = self.tokens.first().unwrap() {
            return vec![
                Box::new(Path::new()),
                Box::new(Empty::new()),
            ]
        }
        return vec![
            Box::new(Number::new()),
            Box::new(MathOp::new()),
            Box::new(StringLexer::new()),
            Box::new(Empty::new())
        ]
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
                    // Look up cmd
                    let paths = env::split_paths(&env::var("PATH").unwrap()).collect();
                    if !SearchFor::matching(token.as_string()).in_paths(paths).is_empty() {
                        return Some(token.clone())
                    }
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

    pub fn get(&mut self, idx: usize) -> Token {
        if idx >= self.tokens.len() {
            if self.scanner_pos < self.data.len() {
                // We have not processed enough
                let _ignored = self.process();
                return self.get(idx)
            } else {
                return Token::Eof
            }
        } else {
            return self.tokens[idx].clone()
        }
    }

    pub fn next(&mut self) -> Result<Token> {
        let token = self.process()?;
        return Ok(token);
    }
}
