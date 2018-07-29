use language::Token;
use language::Number;
use lexer::Lexer;
use std::result;
use std::error::Error;
use std::process::Command;

type Result<T> = result::Result<T, Box<Error>>;

pub struct ParsingState {
    lexer: Lexer,
    token: Token,
}

pub struct Parser {}
impl Parser {

    fn expression(rbp: u32, state: &mut ParsingState) -> Result<String> {
        let mut last = state.token.clone();
        state.token = state.lexer.next();
        let mut left = Parser::nud(last.clone(), state)?;
        println!("l:{}, c:{}, n:{}, rbp:{}", last, state.token, left, rbp);
        while rbp < Token::precedence(state.token.clone()) {
            last = state.token.clone();
            state.token = state.lexer.next();
            println!("l:{}, c:{}, n:{}", last, state.token, left);
            left = Parser::led(left, last.clone(), state)?;
            println!("led:{}", left);
        }
        println!("out:{} rbp:{}, token:{}, prec:{}", left, rbp, state.token,
                 Token::precedence(state.token.clone()));
        return Ok(left)
    }

    pub fn process(string: String) -> Result<String> {
        let mut state = ParsingState {
            lexer: Lexer::new(string)?,
            token: Token::Eof,
        };
        println!("======");
        state.token = state.lexer.next();
        return Parser::expression(0, &mut state);
    }

    fn nud(token: Token, state: &mut ParsingState) -> Result<String> {
        println!("nud => token:{}", token);
        return match token {
            Token::ParO => {
                let res = Parser::expression(Token::precedence(token), state)?;
                state.token = state.lexer.next();//TODO: match
                Ok(res)
            },
            Token::Minus => {
                let mut res = String::from("-");
                res.push_str(Parser::expression(1000, state)?.as_str());
                Ok(res)
            },
            Token::Cmd(path) => {
                let args = Parser::expression(500, state)?;
                println!("cmd args: {}", args);
                let output = if args.is_empty() {
                    Command::new(path).spawn()?
                } else {
                    let mut cmd = Command::new(path);
                    for split in args.split_whitespace() {
                        cmd.arg(split);
                    }
                    cmd.spawn()?

                };
                Ok(String::new())
            }
            Token::Opts(str) => {
                let right = Parser::expression(500, state)?;
                if right.is_empty() {
                    Ok(str)
                } else {
                    let mut res = str.clone();
                    res.push(' ');
                    res.push_str(right.as_str());
                    Ok(res)
                }
            }
            _ => Ok(Token::from(token))
        }
    }

    fn led(left: String, token: Token, state: &mut ParsingState) -> Result<String> {
        println!("led => left:{}, token:{}", left, token);
        return Ok(match token {
            Token::Number(ref str) => str.clone(),
            Token::Opts(ref str) => str.clone(),
            Token::Args(ref str) => str.clone(),
            Token::Plus => {
                let right = Parser::expression(Token::precedence(token), state)?;
                (Number::from(left)? + Number::from(right)?).to()
            },
            Token::Minus => {
                let right = Parser::expression(Token::precedence(token), state)?;
                (Number::from(left)? - Number::from(right)?).to()
            }
            Token::Times => {
                (Number::from(left)? * Number::from(Parser::expression(
                    Token::precedence(token), state)?)?).to()
            },
            Token::Divide => {
                (Number::from(left)? / Number::from(Parser::expression(
                    Token::precedence(token), state)?)?).to()
            },
            _ => left,
        })
    }
}
