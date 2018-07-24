use language::Token;
use language::Number;
use lexer::Lexer;

pub struct ParsingState {
    lexer: Lexer,
    token: Token,
    last: Token,
}

pub struct Parser {}
impl Parser {

    fn expression(rbp: u32, state: &mut ParsingState) -> String {
        state.last = state.token.clone();
        state.token = state.lexer.next();
        let mut left = Parser::nud(state.last.clone());
        println!("last nud: {}", left);
        while rbp < Token::precedence(state.token.clone()) {
            state.last = state.token.clone();
            state.token = state.lexer.next();
            left = Parser::led(left, state.last.clone(), state);
            println!("last led: {}", left);
        }
        return left
    }

    pub fn process(string: String) -> String {
        let mut state = ParsingState {
            lexer: Lexer::new(string),
            token: Token::Eof,
            last: Token::Eof,
        };
        state.token = state.lexer.next();
        return Parser::expression(0, &mut state);
    }

    fn nud(token: Token) -> String {
        return match token {
            Token::Number(str) => str.clone(),
            _ => String::new()
        }
    }

    fn led(left: String, token: Token, state: &mut ParsingState) -> String {
        return match token {
            Token::Plus => {
                let right = Parser::expression(Token::precedence(token), state);
                return (Number::from(left) + Number::from(right)).to()
            },
            Token::Times => {
                return (Number::from(left) * Number::from(Parser::expression(
                    Token::precedence(token), state))).to()
            },
            _ => String::new(),
        }
    }

}
