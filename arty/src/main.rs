extern crate arty;
extern crate termion;

use std::io::{Write, stdout, stdin};

use termion::input::TermRead;
use termion::event::Key;
use termion::raw::IntoRawMode;

use arty::parser;
use arty::parser::ShellContext;
use termion::color;
use std::cmp::max;
use std::cmp::min;

struct Terminal {
    up_count: usize,
}
impl Terminal {
    fn new() -> Terminal {
        return Terminal {
            up_count: 0
        }
    }

    fn reset(&mut self) {
        self.up_count = 0;
    }

    fn format_prompt(ctx: &ShellContext) -> String {
        return format!("{}@{}{}",
                       color::Fg(color::Red),
                       ctx.env.file_name().unwrap().to_str().unwrap(),
                       color::Fg(color::White))
    }

    fn display<W: Write>(out: &mut W, ctx: &ShellContext, line: &String) {
        write!(out, "\r{}", termion::clear::AfterCursor).unwrap();
        write!(out, "{} ", Terminal::format_prompt(&ctx)).unwrap();
        write!(out, "{}", line).unwrap();
        out.flush().unwrap();
    }

    fn read_line(&mut self, ctx: &ShellContext) -> Option<String> {
        self.reset();

        let mut stdout = stdout().into_raw_mode().unwrap();
        let mut line = String::new();
        Terminal::display(&mut stdout, &ctx, &line);


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
                    let history_size = ctx.last.len();
                    let index = history_size - 1 - self.up_count%history_size;
                    line = String::from(ctx.last[index].trim());
                    Terminal::display(&mut stdout, &ctx, &line);
                    self.up_count += 1;
                },
                Key::Down => {
                    let history_size = ctx.last.len();
                    let index = history_size - 1 - self.up_count%history_size;
                    line = String::from(ctx.last[index].trim());
                    Terminal::display(&mut stdout, &ctx, &line);
                    if self.up_count > 0 {
                        self.up_count -= 1;
                    } else if history_size > 0 {
                        self.up_count = history_size - 1
                    }
                },
                Key::Ctrl('d') => return None,
                Key::Ctrl('c') => {
                    self.reset();
                    line.clear();
                    Terminal::display(&mut stdout, &ctx, &line);
                },
                Key::Ctrl('l') => {
                    write!(stdout, "{}{}", termion::clear::All,
                           termion::cursor::Goto(1,1)).unwrap();
                    line.clear();
                    Terminal::display(&mut stdout, &ctx, &line);
                },
                Key::Backspace => {
                    line.pop();
                    Terminal::display(&mut stdout, &ctx, &line);
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
            ctx.last.push(line.clone());
        }
        let res = parser::Parser::process(line, ctx);
        match res {
            Ok(ref str) => {
                if !str.as_string().is_empty() {
                    println!("{}", str.as_string())
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
