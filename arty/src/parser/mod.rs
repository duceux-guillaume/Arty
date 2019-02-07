use language::Number;
use language::Token;
use lexer::Lexer;
use std;
use std::error;
use std::process::Command;
use std::result;
type Result<T> = result::Result<T, Box<error::Error>>;
use std::path::PathBuf;
use feature::shell::Context;

pub struct ParsingState {
    lexer: Lexer,
    token: Token,
}

pub struct Parser {}
impl Parser {
    fn expression(rbp: u32, state: &mut ParsingState, ctx: &mut Context) -> Result<Token> {
        let mut last = state.token.clone();
        state.token = state.lexer.next()?;
        let mut left = Parser::call_expr(last.clone(), state, ctx)?;
        while rbp < state.token.rprec() {
            last = state.token.clone();
            state.token = state.lexer.next()?;
            left = Parser::op_expr(left, last.clone(), state, ctx)?;
        }
        return Ok(left);
    }

    pub fn process(string: String, ctx: &mut Context) -> Result<Token> {
        let mut state = ParsingState {
            lexer: Lexer::new(string),
            token: Token::Eof,
        };
        state.token = state.lexer.next()?;
        return Parser::expression(0, &mut state, ctx);
    }

    fn call_expr(token: Token, state: &mut ParsingState, ctx: &mut Context) -> Result<Token> {
        return match token {
            Token::ParO => {
                let res = Parser::expression(token.lprec(), state, ctx)?;
                state.token = state.lexer.next()?; //TODO: match
                Ok(res)
            }
            Token::Minus => {
                let mut res = String::from("-");
                res.push_str(
                    Parser::expression(token.lprec(), state, ctx)?
                        .as_string()
                        .as_str(),
                );
                Ok(Token::Number(res))
            }
            Token::Cmd(cmd) => {
                let args = Parser::expression(500, state, ctx)?;
                let mut cmd = Command::new(cmd);
                cmd.current_dir(ctx.current_directory().as_path());
                if !args.as_string().is_empty() {
                    for split in args.as_string().split('#') {
                        cmd.arg(split);
                    }
                }
                let status = cmd.status()?;
                if status.success() {
                    Ok(Token::None)
                } else {
                    Err(From::from("Failed".to_string()))
                }
            }
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
            }
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
            }
            Token::ChangeDir => {
                let new_path = PathBuf::from(Parser::expression(500, state, ctx)?.as_string());
                if new_path.to_str().unwrap().is_empty() {
                    ctx.set_current_directory(std::env::home_dir().unwrap());
                } else if new_path.is_absolute() && new_path.exists() {
                    ctx.set_current_directory(new_path);
                } else if !new_path.is_absolute() {
                    let mut tmp = ctx.current_directory().clone();
                    tmp.push(new_path);
                    match tmp.canonicalize() {
                        Ok(path) => ctx.set_current_directory(path),
                        Err(error) => return Err(From::from(error)),
                    }
                } else {
                    return Err(From::from("path doesn't exists".to_string()));
                }
                if !ctx.current_directory().exists() {
                    ctx.set_current_directory(std::env::current_dir()?);
                    Err(From::from("path doesn't exists".to_string()))
                } else {
                    Ok(Token::None)
                }
            }
            _ => Ok(token),
        };
    }

    fn op_expr(
        left: Token,
        token: Token,
        state: &mut ParsingState,
        ctx: &mut Context,
    ) -> Result<Token> {
        return Ok(match token {
            Token::Plus => {
                let right = Parser::expression(token.rprec(), state, ctx)?;
                (Number::from_token(left)? + Number::from_token(right)?).as_token()
            }
            Token::Minus => {
                let right = Parser::expression(token.rprec(), state, ctx)?;
                (Number::from_token(left)? - Number::from_token(right)?).as_token()
            }
            Token::Times => (Number::from_token(left)?
                * Number::from_token(Parser::expression(token.rprec(), state, ctx)?)?).as_token(),
            Token::Divide => (Number::from_token(left)? / Number::from_token(Parser::expression(
                token.rprec(),
                state,
                ctx,
            )?)?).as_token(),
            _ => token,
        });
    }
}
