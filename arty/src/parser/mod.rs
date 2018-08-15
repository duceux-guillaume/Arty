use language::Token;
use language::Number;
use lexer::Lexer;
use std::result;
use std::error;
use std::process::Command;
use std;
type Result<T> = result::Result<T, Box<error::Error>>;
use std::path::PathBuf;

pub struct ShellContext {
    env: PathBuf,
}
impl ShellContext {
    pub fn new() -> Result<Self> {
        return Ok(ShellContext {
            env: std::env::current_dir()?,
        })
    }
}

pub struct ParsingState {
    lexer: Lexer,
    token: Token,
}

pub struct Parser {}
impl Parser {

    fn expression(rbp: u32, state: &mut ParsingState, ctx: &mut ShellContext) -> Result<String> {
        let mut last = state.token.clone();
        state.token = state.lexer.next()?;
        let mut left = Parser::nud(last.clone(), state, ctx)?;
        println!("l:{}, c:{}, n:{}, rbp:{}", last, state.token, left, rbp);
        while rbp < Token::precedence(state.token.clone()) {
            last = state.token.clone();
            state.token = state.lexer.next()?;
            println!("l:{}, c:{}, n:{}", last, state.token, left);
            left = Parser::led(left, last.clone(), state, ctx)?;
            println!("led:{}", left);
        }
        println!("out:{} rbp:{}, token:{}, prec:{}", left, rbp, state.token,
                 Token::precedence(state.token.clone()));
        return Ok(left)
    }

    pub fn process(string: String, ctx: &mut ShellContext) -> Result<String> {
        let mut state = ParsingState {
            lexer: Lexer::new(string)?,
            token: Token::Eof,
        };
        println!("======");
        state.token = state.lexer.next()?;
        return Parser::expression(0, &mut state, ctx);
    }

    fn nud(token: Token, state: &mut ParsingState, ctx: &mut ShellContext) -> Result<String> {
        println!("nud => token:{}", token);
        return match token {
            Token::ParO => {
                let res = Parser::expression(Token::precedence(token), state, ctx)?;
                state.token = state.lexer.next()?;//TODO: match
                Ok(res)
            },
            Token::Minus => {
                let mut res = String::from("-");
                res.push_str(Parser::expression(1000, state, ctx)?.as_str());
                Ok(res)
            },
            Token::Cmd(path) => {
                let args = Parser::expression(500, state, ctx)?;
                println!("cmd args: {}", args);
                println!("cmd env: {}", ctx.env.display());
                let mut cmd = Command::new(path);
                cmd.current_dir(ctx.env.as_path());
                let _output = if args.is_empty() {
                    cmd.spawn()?
                } else {
                    for split in args.split_whitespace() {
                        cmd.arg(split);
                    }
                    cmd.spawn()?

                };
                Ok(String::new())
            },
            Token::Opts(str) | Token::Args(str) | Token::Path(str) => {
                let right = Parser::expression(500, state, ctx)?;
                if right.is_empty() {
                    Ok(str)
                } else {
                    let mut res = str.clone();
                    res.push(' ');
                    res.push_str(right.as_str());
                    Ok(res)
                }
            },
            Token::ChangeDir => {
                let new_path = PathBuf::from(Parser::expression(500, state, ctx)?);
                if new_path.is_absolute() && new_path.exists() {
                    ctx.env = new_path
                } else if !new_path.is_absolute() {
                    let mut tmp = ctx.env.clone();
                    tmp.push(new_path);
                    if tmp.exists() {
                        ctx.env = tmp
                    } else {
                        return Err(From::from("path doesn't exists".to_string()))
                    }
                } else {
                    return Err(From::from("path doesn't exists".to_string()))
                }
                if !ctx.env.exists() {
                    ctx.env = std::env::current_dir()?;
                    Err(From::from("path doesn't exists".to_string()))
                } else {
                    Ok(String::new())
                }
            },
            _ => Ok(Token::from(token))
        }
    }

    fn led(left: String, token: Token, state: &mut ParsingState, ctx: &mut ShellContext) -> Result<String> {
        println!("led => left:{}, token:{}", left, token);
        return Ok(match token {
            Token::Number(ref str) => str.clone(),
            Token::Opts(ref str) => str.clone(),
            Token::Args(ref str) => str.clone(),
            Token::Plus => {
                let right = Parser::expression(Token::precedence(token), state, ctx)?;
                (Number::from(left)? + Number::from(right)?).to()
            },
            Token::Minus => {
                let right = Parser::expression(Token::precedence(token), state, ctx)?;
                (Number::from(left)? - Number::from(right)?).to()
            }
            Token::Times => {
                (Number::from(left)? * Number::from(Parser::expression(
                    Token::precedence(token), state, ctx)?)?).to()
            },
            Token::Divide => {
                (Number::from(left)? / Number::from(Parser::expression(
                    Token::precedence(token), state, ctx)?)?).to()
            },
            _ => left,
        })
    }
}
