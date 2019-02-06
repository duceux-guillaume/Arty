use language::token::Token;
use language::token::Number;
use language::lexer::Lexer;
use std::result;
use std::error;
use std::process::Command;
use std;
type Result<T> = result::Result<T, Box<error::Error>>;
use std::path::PathBuf;

pub struct ShellContext {
    pub env: PathBuf,
    pub last: Vec<String>,
}
impl ShellContext {
    pub fn new() -> Result<Self> {
        return Ok(ShellContext {
            env: std::env::current_dir()?,
            last: Vec::new(),
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
            lexer: Lexer::new(string),
            token: Token::Eof,
        };
        state.token = state.lexer.next()?;
        return Parser::expression(0, &mut state, ctx);
    }

    fn peek(state: &mut ParsingState) -> Token {
        return state.token.clone();
    }


    fn step(state: &mut ParsingState) {
        state.token = state.lexer.next().expect("oh boy!");
    }
    
    fn command(token: String, state: &mut ParsingState, ctx: &mut ShellContext) -> Result<Command> { 
        let mut cmd = Command::new(token);
        cmd.current_dir(ctx.env.as_path());
        while let Token::CmdArgs(arg) = Parser::peek(state) {
            cmd.arg(arg);
            Parser::step(state);
        }
        return Ok(cmd);
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
            Token::Cmd(cmd_tok) => {
                let mut cmd = Parser::command(cmd_tok, state, ctx)?;
                match Parser::peek(state) {
                    Token::Eof => {
                        let status = cmd.status()?;
                        if status.success() {
                            Ok(Token::None)
                        } else {
                            Err(From::from("Failed".to_string()))
                        }
                    },
                    Token::Pipe => {
                       println!("piping");
                       Ok(Token::None)
                       
                    }
                    _ => Err(From::from("Not implemented yet".to_string())),
                }
            },
            Token::CmdArgs(ref str) => {
                let right = Parser::expression(500, state, ctx)?;
                if right.as_string().is_empty() {
                    Ok(token.clone())
                } else {
                    let mut res = str.clone();
                    res.push('#');
                    res.push_str(right.as_string().as_str());
                    Ok(Token::CmdArgs(res))
                }
            },
            Token::Path(ref str) => {
                let right = Parser::expression(500, state, ctx)?;
                if right.as_string().is_empty() {
                    Ok(token.clone())
                } else {
                    let mut res = str.clone();
                    res.push(' ');
                    res.push_str(right.as_string().as_str());
                    Ok(Token::CmdArgs(res))
                }
            },
            Token::ChangeDir => {
                let new_path = PathBuf::from(Parser::expression(500, state, ctx)?.as_string());
                if new_path.to_str().unwrap().is_empty() {
                    ctx.env = std::env::home_dir().unwrap();
                } else if new_path.is_absolute() && new_path.exists() {
                    ctx.env = new_path
                } else if !new_path.is_absolute() {
                    let mut tmp = ctx.env.clone();
                    tmp.push(new_path);
                    match tmp.canonicalize() {
                        Ok(path) => ctx.env = path,
                        Err(error) => return Err(From::from(error)),
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
