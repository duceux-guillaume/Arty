use std::fmt;

pub trait ILexer {
    fn eat(&mut self, c: char) -> State;
    fn reset(&mut self);
    fn token(&mut self) -> Token;
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum State {
    ACC,
    ONG,
    REJ,
    STA
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum Type {
    ERROR,
    NUMBER,
    STRING,
    IDENTIFIER,
    MATHOP,
    CRTLOP,
}

impl fmt::Display for Type {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match *self {
            Type::ERROR => write!(f, "ERROR"),
            Type::NUMBER => write!(f, "NUMBER"),
            Type::STRING => write!(f, "STRING"),
            Type::IDENTIFIER => write!(f, "IDENTIFIER"),
            Type::MATHOP => write!(f, "MATHOP"),
            Type::CRTLOP => write!(f, "CRTLOP"),
        }
    }
}

#[derive(Clone, Debug, PartialEq)]
pub struct Token {
    typ: Type,
    val: String,
}

impl Token {
    pub fn new(typ: Type, val: String) -> Self {
        return Token {
            typ,
            val,
        }
    }

    pub fn push(&mut self, c: char) {
        self.val.push(c)
    }

    pub fn get_type(&self) -> Type {
        return self.typ.clone()
    }

    pub fn get_val(&self) -> String {
        return self.val.clone()
    }
}

impl fmt::Display for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
       write!(f, "{}({})", self.typ, self.val)
    }
}

