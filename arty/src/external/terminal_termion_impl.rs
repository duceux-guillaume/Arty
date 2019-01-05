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

    fn clear(&self) {
        let mut stdout = stdout().into_raw_mode().unwrap();
        write!(stdout, "{}{}", termion::clear::All,
               termion::cursor::Goto(1,1)).unwrap();
        stdout.flush().unwrap();
    }
}

pub struct TermionKeyboard {}

impl Keyboard for TermionKeyboard {
    fn get_key(&self) -> Key {
        let mut _stdout = stdout().into_raw_mode().unwrap();
        let stdin = stdin();
        return match stdin.keys().next()
                .unwrap()
                .expect("couldn't read stdin")
                {
            TermKey::Char(charr) => {
                Key::Char(charr)
            }
            _ => {
                Key::Esc
            }
        }
    }
}

