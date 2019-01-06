extern crate termion;

use core::terminal::Terminal;
use core::terminal::Keyboard;
use core::terminal::Key;

use std::io::{Write, stdout, stdin};

use self::termion::input::TermRead;
use self::termion::event::Key as TermKey;
use self::termion::raw::IntoRawMode;
use self::termion::color;

pub struct TermionTerminal {
    prompt: String,
    pos: usize
}

impl TermionTerminal {
    pub fn new() -> TermionTerminal {
        return TermionTerminal {
            prompt: String::new(),
            pos: 0,
        }
    }
}

impl Terminal for TermionTerminal {
    fn write_prompt(&mut self, line: String) {
        self.prompt = line;
    }

    fn write_line(&self, line: String) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "\r{}", termion::clear::CurrentLine).unwrap();
        write!(stdout, "{} ", self.prompt);
        write!(stdout, "{}", line).unwrap();
        let offset = line.len() - self.pos;
        if offset > 0 {
            write!(stdout, "{}", termion::cursor::Left(offset as u16)).unwrap();
        }
        stdout.flush().unwrap();
    }

    fn write_guesses(&self, guesses: Vec<String>) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "\n\r{}", termion::clear::CurrentLine).unwrap();
        for guess in guesses.iter() {
            write!(stdout, "{} ", guess.to_string()).unwrap();
        }
        write!(stdout, "{}\r", termion::cursor::Up(1)).unwrap();
        stdout.flush().unwrap();
    }

    fn write_new_line(&self) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "{}", "\r\n").unwrap();
        stdout.flush().unwrap();
    }

    fn write_cursor(&mut self, pos: usize) {
        self.pos = pos;
    }

    fn clear(&self) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "{}{}", termion::clear::All,
               termion::cursor::Goto(1,1)).unwrap();
        stdout.flush().unwrap();
    }
}

pub struct TermionKeyboard {}

impl TermionKeyboard {
    pub fn new() -> TermionKeyboard {
        return TermionKeyboard {}
    }
}

impl Keyboard for TermionKeyboard {
    fn get_key(&self) -> Key {
        let mut _stdout = stdout().into_raw_mode().unwrap();
        let stdin = stdin();
        return match stdin.keys().next()
                .unwrap()
                .expect("couldn't read stdin")
                {
            TermKey::Char(charr) => {
                if charr == '\n' {
                    Key::Enter
                } else if charr == '\t' {
                    Key::Tab
                } else {
                    Key::Char(charr)
                }
            },
            TermKey::Left => Key::Left,
            TermKey::Right => Key::Right,
            TermKey::Up => Key::Up,
            TermKey::Down => Key::Down,
            TermKey::Ctrl(key) => Key::Ctrl(key),
            TermKey::Alt(key) => Key::Alt(key),
            TermKey::Esc => Key::Esc,
            TermKey::Backspace => Key::Backspace,
            TermKey::Delete => Key::Delete,
            _ => {
                Key::Esc
            }
        }
    }
}

