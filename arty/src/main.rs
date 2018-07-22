
mod lexer {

    pub fn is_digit(c: char) -> bool {
        return c >= '0' && c <= '9'
    }

    pub fn is_blanck(c: char) -> bool {
        return c == ' ' || c == '\n'
    }
    
    enum Type {
        NUMBER,
        STRING,
    }

    struct Token {
        typ: Type,
        val: String,
    }

    impl Token {
        pub fn new(typ: Type) -> Self {
            return Token {
                typ,
                val: String::new(),
            }
        }
        pub fn push(&mut self, c: char) {
            self.val.push(c)
        }
    }

    #[derive(Copy, Clone)]
    pub enum State {
        ACC,
        ONG,
        REJ,
        STA
    }

    use std::fmt;
    impl fmt::Display for State {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            match *self {
                State::STA => write!(f, "{}", "STA"),
                State::ACC => write!(f, "{}", "ACC"),
                State::ONG => write!(f, "{}", "ONG"),
                _ => write!(f, "{}", "REJ"),
            }
        }
    }

    pub trait ILexer {
        fn eat(&mut self, c: char) -> State {
            return State::REJ
        }
    }
    
    pub struct NumberLexer {
        token: Token,
        state: State,
    }

    impl NumberLexer {
        pub fn new() -> Self {
            return NumberLexer {
                token: Token::new(Type::NUMBER),
                state: State::STA,
            }
        }
    }
    impl ILexer for NumberLexer {
        fn eat(&mut self, c: char) -> State {
            match self.state {
                State::REJ => return self.state.clone(),
                State::ACC => return self.state.clone(),
                _ => {
                    if is_digit(c) {
                        self.token.push(c);
                        self.state = State::ONG;
                        return self.state.clone()
                    }
                    self.state = if is_blanck(c) {
                        State::ACC
                    } else {
                        State::REJ
                    };
                    return self.state.clone()
                }
            }
        } 
    }

    pub struct Lexer {
        automatas: Vec<Box<ILexer>>,
    }

    impl Lexer {
        pub fn new() -> Self {
            let mut lexer = Lexer {
                automatas: Vec::new()
            };
            lexer.automatas.push(Box::new(NumberLexer::new()));
            return lexer
        }
    }
}

use lexer::ILexer;

fn main() {
    println!("Hello, world!");

    let mut line = String::new();
    std::io::stdin().read_line(&mut line)
        .expect("Failed to read line");

    let mut lexer = lexer::NumberLexer::new();
    for c in line.chars() {
        println!("{} {}", c, lexer.eat(c));
    }
}
