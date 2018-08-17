extern crate arty;
extern crate termion;

use std::io::{Write, stdout, stdin};

use termion::input::TermRead;
use termion::event::Key;
use termion::raw::IntoRawMode;

use arty::parser;
use arty::parser::ShellContext;
use termion::color;

struct Terminal {
    up_count: usize,
    left_count: usize,
}
impl Terminal {
    fn new() -> Terminal {
        return Terminal {
            up_count: 0,
            left_count: 0
        }
    }

    fn reset(&mut self) {
        self.up_count = 0;
        self.left_count = 0;
    }

    fn format_prompt(ctx: &ShellContext) -> String {
        return format!("{}@{}{}",
                       color::Fg(color::Red),
                       ctx.env.file_name().unwrap().to_str().unwrap(),
                       color::Fg(color::White))
    }

    fn display<W: Write>(&self, out: &mut W, ctx: &ShellContext, line: String) {
        write!(out, "\r{}", termion::clear::AfterCursor).unwrap();
        write!(out, "{} ", Terminal::format_prompt(&ctx)).unwrap();
        write!(out, "{}", line).unwrap();
        if self.left_count > 0 {
            write!(out, "{}", termion::cursor::Left(self.left_count as u16)).unwrap();
        }
        out.flush().unwrap();
    }

    fn read_line(&mut self, ctx: &ShellContext) -> Option<String> {
        self.reset();

        let mut stdout = stdout().into_raw_mode().unwrap();
        let mut line = Vec::new();
        let stdin = stdin();

        self.display(&mut stdout, &ctx, line.iter().collect());
        for key in stdin.keys() {
            match key.unwrap() {
                Key::Char(character) => {
                    if line.len() == 0 || self.left_count == 0 || character == '\n' {
                        line.push(character)
                    } else {
                        let index = line.len() - self.left_count;
                        line.insert(index, character);
                    }
                    if character == '\n' {
                        write!(stdout, "{}", "\r\n").unwrap();
                        return Some(line.iter().collect())
                    }
                },
                Key::Up => {
                    let history_size = ctx.last.len();
                    if history_size > 0 {
                        let index = history_size - 1 - self.up_count % history_size;
                        line = ctx.last[index].trim().chars().collect();
                        self.up_count += 1;
                        self.left_count = 0;
                    }
                },
                Key::Down => {
                    let history_size = ctx.last.len();
                    if history_size > 0 {
                        let index = history_size - 1 - self.up_count % history_size;
                        line = ctx.last[index].trim().chars().collect();
                        if self.up_count > 0 {
                            self.up_count -= 1;
                        } else {
                            self.up_count = history_size - 1
                        }
                        self.left_count = 0;
                    }
                },
                Key::Left => {
                    if self.left_count < line.len() {
                        self.left_count += 1;
                    }
                },
                Key::Right => {
                    if self.left_count > 0 {
                        self.left_count -= 1;
                    }
                },
                Key::Ctrl('d') => return None,
                Key::Ctrl('c') => {
                    self.reset();
                    line.clear();
                },
                Key::Esc => {
                    self.reset();
                    line.clear();
                },
                Key::Ctrl('l') => {
                    write!(stdout, "{}{}", termion::clear::All,
                           termion::cursor::Goto(1,1)).unwrap();
                    line.clear();
                    self.reset();
                },
                Key::Backspace => {
                    if line.len() > 0 && self.left_count < line.len() {
                        let index = line.len() - 1 - self.left_count;
                        line.remove(index);
                    }
                },
                Key::Delete => {
                    if line.len() > 0 && self.left_count > 0 && self.left_count <= line.len() {
                        let index = line.len() - self.left_count;
                        line.remove(index);
                        if self.left_count > 0 {
                            self.left_count -= 1;
                        }                    }
                },
                _ => {}
            }
            self.display(&mut stdout, &ctx, line.iter().collect());
        }
        return Some(line.iter().collect());
    }
}

struct Interpreter {}
impl Interpreter {
    fn new() -> Interpreter {
        return Interpreter {}
    }

    fn process(&mut self, line: String, ctx: &mut ShellContext) {
        if !line.trim().is_empty() {
            if ctx.last.len() == 0 || line != *ctx.last.last().unwrap() {
                ctx.last.push(line.clone());
            }
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
