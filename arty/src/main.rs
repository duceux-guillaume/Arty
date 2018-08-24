extern crate arty;
extern crate termion;

use std::io::{Write, stdout, stdin};
use std::path::PathBuf;

use termion::input::TermRead;
use termion::event::Key;
use termion::raw::IntoRawMode;

use arty::parser;
use arty::parser::ShellContext;
use termion::color;
use arty::lexer::Lexer;
use arty::language::Token;

struct Guess {
    first_part: Vec<char>,
    missing_part: Vec<char>
}
impl Guess {
    fn new(first: &str, second: &str) -> Guess {
        return Guess {
            first_part: first.chars().collect(),
            missing_part: second.chars().collect(),
        }
    }

    fn missing_part(&self) -> Vec<char> {
        let mut res = self.missing_part.clone();
        res.push('/');
        return res
    }

    fn to_string(&self) -> String {
        let first: String = self.first_part.iter().collect();
        let second: String = self.missing_part.iter().collect();
        return format!("{}{}", first, second)
    }
}

struct PathGuesser {}
impl PathGuesser {
    fn new() -> PathGuesser {
        return PathGuesser{}
    }

    fn guess(&self, ctx: &ShellContext, line: &Vec<char>) -> Vec<Guess> {
        let mut lexer = Lexer::new(line.iter().collect());
        let mut result = Vec::new();
        // Check first token
        if let Token::ChangeDir = lexer.get(0) {
            // Then second
            match lexer.get(1) {
                Token::None | Token::Eof => {
                    result = self.convert(self.list_directory(&ctx.env));
                },
                Token::Path(hint) => {
                    // we have arguments, let's figure out where the user went
                    let path_hint = PathBuf::from(hint.clone());
                    if !path_hint.is_absolute() {
                        let mut env = ctx.env.clone();
                        env.push(path_hint.clone());
                        result = self.guess_directory_from_hint(env);
                    } else {
                        result = self.guess_directory_from_hint(path_hint);
                    }
                },
                _ => {
                    println!("what?")
                },
            }
        }
        return result
    }

    fn guess_directory_from_hint(&self, hint: PathBuf) -> Vec<Guess> {
        return if hint.exists() {
            self.convert(self.list_directory(&hint))
        } else {
            // no arguments yet, return context directory
            let missing_hint = hint.file_name().unwrap().to_str().unwrap();
            self.filter_match(missing_hint, self.list_partial_directory(&hint))
        }
    }

    fn list_directory(&self, ctx: &PathBuf) -> Vec<String> {
        let mut result = Vec::new();
        if ctx.is_dir() {
            for entry in ctx.read_dir().expect("can't list dir") {
                let path = entry.expect("whatever").path();
                if path.is_dir() {
                    result.push(String::from(path.file_name().unwrap().to_str().unwrap()));
                }
            }
        }
        return result
    }

    fn list_partial_directory(&self, ctx: &PathBuf) -> Vec<String> {
        return if ctx.exists() {
            self.list_directory(ctx)
        } else {
            println!("\ndir: {}", ctx.parent().unwrap().to_str().unwrap());
            self.list_directory(&ctx.parent().unwrap().to_path_buf())
        }
    }

    fn filter_match(&self, pattern: &str, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            if item.contains(pattern) {
                let splits: String = item.split(pattern).collect();
                result.push(Guess::new(pattern, splits.as_str()));
            }
        }
        return result
    }

    fn convert(&self, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            result.push(Guess::new("", item.as_str()));
        }
        return result
    }
}

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
                        let guesser = PathGuesser::new();
                        let guesses = guesser.guess(ctx, &line);
                        if guesses.len() == 1 {
                            line.append(&mut guesses.first().unwrap().missing_part());
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
