extern crate arty;
extern crate termion;

use std::io::{Write, stdout, stdin};

use termion::input::TermRead;
use termion::event::Key;
use termion::raw::IntoRawMode;

use arty::parser;
use arty::parser::ShellContext;

struct Terminal {}
impl Terminal {
    fn new() -> Terminal {
        return Terminal {}
    }

    fn get_prompt(ctx: &mut ShellContext) -> String {
        return format!("{}", ctx.env.to_str().unwrap())
    }

    fn read_line(&mut self, ctx: &mut ShellContext) -> Option<String> {

        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "{} ", Terminal::get_prompt(ctx));
        stdout.flush().unwrap();

        let mut line = String::new();

        let stdin = stdin();
        for key in stdin.keys() {
            match key.unwrap() {
                Key::Char(character) => {
                    line.push(character);
                    if let '\n' = character {
                        write!(stdout, "{}", "\r\n").unwrap();
                        return Some(line)
                    } else {
                        write!(stdout, "{}", character).unwrap();
                    }
                },
                Key::Up => {
                    write!(stdout, "\r{}", termion::clear::AfterCursor).unwrap();
                    write!(stdout, "{} ", Terminal::get_prompt(ctx));
                    line = String::from(ctx.last.trim());
                    write!(stdout, "{}", line);
                },
                Key::Ctrl('c') => return None,
                Key::Ctrl('l') => {
                    write!(stdout, "{}{}", termion::clear::All,
                           termion::cursor::Goto(1,1)).unwrap();
                    write!(stdout, "{} ", Terminal::get_prompt(ctx));
                    line.clear();
                },
                Key::Backspace => {
                    write!(stdout, "\r{}", termion::clear::AfterCursor).unwrap();
                    write!(stdout, "{} ", Terminal::get_prompt(ctx));
                    line.pop();
                    write!(stdout, "{}", line);
                },
                _ => {}
            }
            stdout.flush().unwrap();
        }
        return Some(line);
    }
}

struct Interpreter {}
impl Interpreter {
    fn new() -> Interpreter {
        return Interpreter {}
    }

    fn process(&mut self, line: String, ctx: &mut ShellContext) {
        if !line.trim().is_empty() {
            ctx.last = line.clone();
        }
        let res = parser::Parser::process(line, ctx);
        match res {
            Ok(ref str) => {
                if !str.is_empty() {
                    println!("{}", str)
                }
            },
            Err(ref str) => println!("{}", str),
        }
    }
}

fn main() {
    println!("Hello, world!");

    let mut ctx= match ShellContext::new() {
        Ok(shell_ctx) => shell_ctx,
        _ => panic!("no env"),
    };
    let mut arty = Interpreter::new();
    let mut tty = Terminal::new();
    loop {
        let line = tty.read_line(&mut ctx);
        match line {
            Some(string) => arty.process(string, &mut ctx),
            None => break,
        }
    }
}
