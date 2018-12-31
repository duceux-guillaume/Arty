extern crate arty;
extern crate termion;

use std::io::{Write, stdout, stdin};

use termion::input::TermRead;
use termion::event::Key;
use termion::raw::IntoRawMode;

use arty::parser;
use arty::parser::ShellContext;
use termion::color;
use arty::guesser::Guess;
use arty::guesser::PathGuesser;
use arty::guesser::FileGuesser;
use arty::external::UserHistoryFileCreator;
use arty::core::UserHistoryFile;

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

    fn write_line(&self, ctx: &ShellContext, line: String) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "\r{}", termion::clear::CurrentLine).unwrap();
        write!(stdout, "{} ", Terminal::format_prompt(&ctx)).unwrap();
        write!(stdout, "{}", line).unwrap();
        if self.left_count > 0 {
            write!(stdout, "{}", termion::cursor::Left(self.left_count as u16)).unwrap();
        }
        stdout.flush().unwrap();
    }

    fn write_guesses(&self, guesses: Vec<Guess>) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "\n\r{}", termion::clear::CurrentLine).unwrap();
        for guess in guesses.iter() {
            write!(stdout, "{} ", guess.to_string()).unwrap();
        }
        write!(stdout, "{}\r", termion::cursor::Up(1)).unwrap();
        stdout.flush().unwrap();
    }

    fn write_newline(&self) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "{}", "\r\n").unwrap();
        stdout.flush().unwrap();
    }

    fn clear(&self) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "{}{}", termion::clear::All,
               termion::cursor::Goto(1,1)).unwrap();
        stdout.flush().unwrap();
    }

    fn get_key(&self) -> Key {
        let mut _stdout = stdout().into_raw_mode().unwrap();
        let stdin = stdin();
        return stdin.keys().next().unwrap().expect("whatever")
    }

    fn read_line(&mut self, ctx: &ShellContext) -> Option<String> {
        self.reset();

        let mut line = Vec::new();

        self.write_line(&ctx, line.iter().collect());
        loop {
            match self.get_key() {
                Key::Char(character) => {
                    if character == '\t' {
                        let mut guesser = PathGuesser::new();
                        let mut guesses = guesser.guess(ctx, &line);
                        if guesses.len() == 1 {
                            line.append(&mut guesses.first().unwrap().missing_part());
                        } else if guesses.len() == 0 {
                            let file_guesser = FileGuesser::new();
                            guesses = file_guesser.guess(ctx, &line);
                            if guesses.len() == 1 {
                                line.append(&mut guesses.first().unwrap().missing_part());
                            }
                        }
                        self.write_guesses(guesses);
                    } else {
                        if line.len() == 0 || self.left_count == 0 || character == '\n' {
                            line.push(character)
                        } else {
                            let index = line.len() - self.left_count;
                            line.insert(index, character);
                        }
                        if character == '\n' {
                            self.write_newline();
                            return Some(line.iter().collect())
                        }
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
                    self.clear();
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
                        }
                    }
                },
                _ => {}
            }
            self.write_line(&ctx, line.iter().collect());
        }
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
        let res = parser::Parser::process(line.clone(), ctx);
        match res {
            Ok(ref str) => {
                let mut history = UserHistoryFile::new(UserHistoryFileCreator::create_default_file());
                history.record(line.as_ref());
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
    UserHistoryFileCreator::create_default_file();


    let mut ctx= ShellContext::new().expect("no suitable env");
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
