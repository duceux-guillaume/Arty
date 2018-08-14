use lexer::interface::ILexer;
use lexer::interface::State;
use language::Token;
use lexer::helper;

pub struct Cmd {
    token: String,
    state: State,
}

impl Cmd {
    pub fn new() -> Self {
        return Cmd {
            token: String::new(),
            state: State::Sta,
        }
    }
}

impl ILexer for Cmd {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                if helper::is_path_char(c) {
                    self.token.push(c);
                    State::Ong
                } else {
                    State::Rej
                }
            },
            _ => {
                if helper::is_path_char(c) {
                    self.token.push(c);
                    State::Ong
                } else if helper::is_blank(c) {
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
        return Token::Cmd(self.token.clone())
    }
}

pub struct Path {
    token: String,
    state: State,
}
impl Path {
    pub fn new() -> Self {
        return Path {
            token: String::new(),
            state: State::Sta,
        }
    }
}
impl ILexer for Path {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                if helper::is_path_char(c) {
                    self.token.push(c);
                    State::Ong
                } else {
                    State::Rej
                }
            },
            _ => {
                if helper::is_path_char(c) {
                    self.token.push(c);
                    State::Ong
                } else if helper::is_blank(c) {
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
        return Token::Path(self.token.clone())
    }
}

pub struct StringLexer {
    token: String,
    state: State,
}
impl StringLexer {
    pub fn new() -> Self {
        return StringLexer {
            token: String::new(),
            state: State::Sta,
        }
    }
}
impl ILexer for StringLexer {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                if c == '"' {
                    self.token.push(c);
                    State::Ong
                } else {
                    State::Rej
                }
            },
            _ => {
                if c == '"' {
                    self.token.push(c);
                    State::Acc
                } else {
                    self.token.push(c);
                    State::Ong
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
        return Token::String(self.token.clone())
    }
}

pub struct Empty {
    state: State
}
impl Empty {
    pub fn new() -> Self {
        return Empty { state: State::Acc }
    }
}
impl ILexer for Empty {
    fn eat(&mut self, c: char) -> State {
        let new_state = match self.state {
            State::Sta => {
                if helper::is_blank(c) {
                    State::Ong
                } else {
                    State::Rej
                }
            },
            State::Ong => {
                if helper::is_blank(c) {
                    State::Ong
                } else {
                    State::Acc
                }
            },
            _ => self.state,
        };
        self.state = new_state;
        return self.state
    }

    fn reset(&mut self) {
        self.state = State::Sta;
    }

    fn token(&mut self) -> Token {
        return Token::None
    }
}

pub struct Opts {
    token: String,
    state: State,
}

impl Opts {
    pub fn new() -> Self {
        return Opts {
            token: String::new(),
            state: State::Sta,
        }
    }
}

impl ILexer for Opts {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                if c == '-' {
                    self.token.push(c);
                    State::Ong
                } else {
                    State::Rej
                }
            },
            _ => {
                if c == '-' || helper::is_letter(c) {
                    self.token.push(c);
                    State::Ong
                } else if helper::is_blank(c) || c == '\"' {
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
        return Token::Opts(self.token.clone())
    }
}

pub struct ChangeDir {
    state: State,
}

impl ChangeDir {
    pub fn new() -> Self {
        return ChangeDir {
            state: State::Sta,
        }
    }
}

impl ILexer for ChangeDir {
    fn eat(&mut self, c: char) -> State {
        let new_state: State = match self.state {
            State::Rej => self.state,
            State::Acc => self.state,
            State::Sta => {
                if c == 'c' {
                    State::Ong
                } else {
                    State::Rej
                }
            },
            _ => {
                if c == 'd' {
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
        self.state = State::Sta;
    }

    fn token(&mut self) -> Token {
        return Token::ChangeDir
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn integer() {
        let string = String::from("1234567890");
        let mut lexer = Cmd::new();
        for c in string.chars() {
            assert_eq!(State::Rej, lexer.eat(c));
        }
        assert_eq!(State::Rej, lexer.eat(' '));
    }

    #[test]
    fn identifier() {
        let string = String::from("a1");
        let mut lexer = Cmd::new();
        for c in string.chars() {
            assert_eq!(State::Ong, lexer.eat(c));
        }
        assert_eq!(State::Acc, lexer.eat(' '));
    }

    #[test]
    fn identifier2() {
        let string = String::from("aname");
        let mut lexer = Cmd::new();
        for c in string.chars() {
            assert_eq!(State::Ong, lexer.eat(c));
        }
        assert_eq!(State::Acc, lexer.eat(' '));
    }

    #[test]
    fn blanck() {
        let string = String::from(" ");
        let mut lexer = Cmd::new();
        for c in string.chars() {
            assert_eq!(State::Rej, lexer.eat(c));
        }
        assert_eq!(State::Rej, lexer.eat(' '));
    }
}
