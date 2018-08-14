use language::Token;
use language::Number;
use lexer::Lexer;
use std::result;
use std::error;
use std::process::Command;
use std::process::Stdio;
use std;
type Result<T> = result::Result<T, Box<error::Error>>;
use std::path::PathBuf;

pub struct ShellContext {
    pub env: PathBuf,
    pub last: String,
}
impl ShellContext {
    pub fn new() -> Result<Self> {
        return Ok(ShellContext {
            env: std::env::current_dir()?,
            last: String::new(),
        })
    }
}

pub struct ParsingState {
    lexer: Lexer,
    token: Token,
}

pub struct Parser {}
impl Parser {

    fn expression(rbp: u32, state: &mut ParsingState, ctx: &mut ShellContext) -> Result<Token> {
        let mut last = state.token.clone();
        state.token = state.lexer.next()?;
        let mut left = Parser::call_expr(last.clone(), state, ctx)?;
        while rbp < state.token.rprec() {
            last = state.token.clone();
            state.token = state.lexer.next()?;
            left = Parser::op_expr(left, last.clone(), state, ctx)?;
        }
        return Ok(left)
    }

    pub fn process(string: String, ctx: &mut ShellContext) -> Result<Token> {
        let mut state = ParsingState {
            lexer: Lexer::new(string)?,
            token: Token::Eof,
        };
        state.token = state.lexer.next()?;
        return Parser::expression(0, &mut state, ctx);
    }


    fn call_expr(token: Token, state: &mut ParsingState, ctx: &mut ShellContext) -> Result<Token> {
        return match token {
            Token::ParO => {
                let res = Parser::expression(token.lprec(), state, ctx)?;
                state.token = state.lexer.next()?;//TODO: match
                Ok(res)
            },
            Token::Minus => {
                let mut res = String::from("-");
                res.push_str(Parser::expression(token.lprec(), state, ctx)?.as_string().as_str());
                Ok(Token::Number(res))
            },
            Token::Cmd(path) => {
                let args = Parser::expression(500, state, ctx)?;
                let mut cmd = Command::new(path);
                cmd.current_dir(ctx.env.as_path())
                    .stdout(Stdio::inherit())
                    .stdin(Stdio::inherit());
                if !args.as_string().is_empty() {
                    for split in args.as_string().split_whitespace() {
                        cmd.arg(split);
                    }
                }
                let _output = cmd.output();
                Ok(Token::None)
            },
            Token::Opts(ref str) | Token::Args(ref str) | Token::Path(ref str) => {
                let right = Parser::expression(500, state, ctx)?;
                if right.as_string().is_empty() {
                    Ok(token.clone())
                } else {
                    let mut res = str.clone();
                    res.push(' ');
                    res.push_str(right.as_string().as_str());
                    Ok(Token::Args(res))
                }
            },
            Token::ChangeDir => {
                let new_path = PathBuf::from(Parser::expression(500, state, ctx)?.as_string());
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
                    Ok(Token::None)
                }
            },
            _ => Ok(token)
        }
    }

    fn op_expr(left: Token, token: Token, state: &mut ParsingState, ctx: &mut ShellContext) -> Result<Token> {
        return Ok(match token {
            Token::Plus => {
                let right = Parser::expression(token.rprec(), state, ctx)?;
                (Number::from_token(left)? + Number::from_token(right)?).as_token()
            },
            Token::Minus => {
                let right = Parser::expression(token.rprec(), state, ctx)?;
                (Number::from_token(left)? - Number::from_token(right)?).as_token()
            }
            Token::Times => {
                (Number::from_token(left)? * Number::from_token(Parser::expression(
                    token.rprec(), state, ctx)?)?).as_token()
            },
            Token::Divide => {
                (Number::from_token(left)? / Number::from_token(Parser::expression(
                    token.rprec(), state, ctx)?)?).as_token()
            },
            _ => token,
        })
    }
}
