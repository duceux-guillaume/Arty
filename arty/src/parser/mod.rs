use language::Token;
use lexer::Lexer;

trait TokenParser {
    fn from_token(token: Token) -> Box<TokenParser> where Self: Sized;
    fn nud(&self) -> String;
    fn lbp(&self) -> u64;
    fn led(&self, state: &mut ParsingState, value: String) -> String;
}

struct ErrorParser {
    token: Token
}

impl TokenParser for ErrorParser {
    fn from_token(token: Token) -> Box<TokenParser> {
        return Box::new(ErrorParser {
            token,
        })
    }

    fn nud(&self) -> String {
        return Token::from(self.token.clone())
    }

    fn lbp(&self) -> u64 {
        return 0
    }

    fn led(&self, state: &mut ParsingState, value: String) -> String {
        return value
    }
}

struct IdentifierParser {
    value: String
}

impl TokenParser for IdentifierParser {
    fn from_token(token: Token) -> Box<TokenParser> {
        return Box::new(IdentifierParser {
            value: Token::from(token),
        })
    }

    fn nud(&self) -> String {
        return self.value.clone()
    }

    fn lbp(&self) -> u64 {
        return 1
    }

    fn led(&self, state: &mut ParsingState, left: String) -> String {
        let right = Parser::expression(self.lbp(), state);
        let mut res = left.clone();
        res.push_str(" ");
        res.push_str(self.value.as_str());
        res.push_str(" ");
        res.push_str(right.as_str());
        return res
    }
}

struct EndParser {}
impl TokenParser for EndParser {
    fn from_token(_token: Token) -> Box<TokenParser> where Self: Sized {
        return Box::new(EndParser{})
    }

    fn nud(&self) -> String {
        return String::from("");
    }

    fn lbp(&self) -> u64 {
        return 0
    }

    fn led(&self, state: &mut ParsingState, value: String) -> String {
        return value
    }
}

pub struct ParsingState {
    tokens: Vec<Token>,
    token: usize,
    last: usize,
}

pub struct Parser {}
impl Parser {
    pub fn new() -> ParsingState {
        return ParsingState {
            tokens: Vec::new(),
            token: 0,
            last: 0
        }
    }

    pub fn process(string: String) -> String {
        let mut lexer = Lexer::new();
        let mut state = Parser::new();
        state.tokens = lexer.process(string);
        println!("------");
        for l in state.tokens.iter() {
            println!("{}", l);
        }
        println!("------");
        return Parser::expression(0, &mut state);
    }

    fn get_parser(next: usize, state: &mut ParsingState) -> Box<TokenParser> {
        if next >= state.tokens.len() {
            return Parser::end_parser()
        }
        let t = state.tokens[next].clone();
        return match t {
            Token::IDENTIFIER(ref _str) => IdentifierParser::from_token(t.clone()),
            _ => ErrorParser::from_token(t),
        }
    }

    fn expression(rbp: u64, state: &mut ParsingState) -> String {
        state.last = state.token;
        state.token += 1;
        let mut left = Parser::get_parser(state.last, state).nud();
        println!("last nud: {}", left);
        while rbp < Parser::get_parser(state.token, state).lbp() {
            state.last = state.token;
            state.token += 1;
            left = Parser::get_parser(state.last, state).led(state, left);
            println!("last led: {}", left);
        }
        return left
    }


    fn end_parser() -> Box<TokenParser> {
        return Box::new(EndParser{})
    }
}
