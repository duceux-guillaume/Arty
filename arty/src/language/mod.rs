use std::fmt;

#[derive(Clone, PartialEq)]
pub enum Token {
    EOF,
    ERROR(String),
    NUMBER(String),
    STRING(String),
    IDENTIFIER(String),
    // Math operations
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    MODULO,
    // CTRL OP
    SEQUENCIAL,
    BACKGROUND,
}

impl Token {
    pub fn precedence(token: Token) -> u32 {
        return match token {
            EOF => 0,
            ERROR => 0,
            // Math OP
            PLUS => 100,
            MINUS => 100,
            TIMES => 101,
            DIVIDE => 101,
            MODULO => 102,
            _ => 0,
        }
    }

    pub fn from(token: Token) -> String {
        return match token {
            Token::ERROR(str) => str,
            Token::NUMBER(str) => str,
            Token::STRING(str) => str,
            Token::IDENTIFIER(str) => str,
            _ => String::new(),
        }
    }
}

impl fmt::Display for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match *self {
            Token::ERROR(ref str) => write!(f, "ERROR({})", str),
            Token::NUMBER(ref str) => write!(f, "NUMBER({})", str),
            Token::STRING(ref str) => write!(f, "STRING({})", str),
            Token::IDENTIFIER(ref str) => write!(f, "IDENTIFIER({})", str),
            _ => write!(f, "NA"),
        }
    }
}

