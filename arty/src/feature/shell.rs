use std::rc::Rc;

use core::guesser::GuesserManager;
use core::terminal::Key;
use core::terminal::Keyboard;
use core::terminal::Terminal;

use feature::interpreter::Interpreter;

pub struct Context {
    current_directory: std::path::PathBuf,
}
impl Context {
    pub fn new() -> Context {
        return Context {
            current_directory: std::env::current_dir().unwrap(),
        }
    }

    pub fn current_directory(&self) -> &std::path::PathBuf {
        return &self.current_directory;
    }

    pub fn set_current_directory(&mut self, path: std::path::PathBuf) {
        self.current_directory = path;
    }
}


pub struct ShellController {
    buffer: Vec<char>,
    insert_index: usize,
    keyboard: Box<Keyboard>,
    terminal: Box<Terminal>,
    guesser: GuesserManager,
    interpreter: Interpreter,
    context: Rc<Context>,
}
impl ShellController {
    fn reset(&mut self) {
        self.buffer = Vec::new();
        self.insert_index = 0;
        self.guesser.process(self.buffer.iter().collect());
    }

    pub fn run(&mut self) {
        loop {
            self.reset();
            if !self.read_user_input() {
                println!("bye bye");
                return;
            }
            self.interpreter.process(self.buffer.iter().collect(), Rc::clone(&self.context));
        }
    }

    fn read_user_input(&mut self) -> bool {
        loop {
            self.terminal.write_guesses(self.guesser.to_string_vec());
            self.terminal.write_cursor(self.insert_index);
            self.terminal.write_line(self.buffer.iter().collect());
            match self.keyboard.get_key() {
                Key::Char(charr) => {
                    self.buffer.insert(self.insert_index, charr);
                    self.insert_index += 1;
                    self.guesser.process(self.buffer.iter().collect());
                }
                Key::Left => {
                   if self.insert_index > 0 {
                        self.insert_index -= 1;
                    }
                }
                Key::Right => {
                    if self.insert_index < self.buffer.len() {
                        self.insert_index += 1;
                    }
                }
                Key::Backspace => {
                    if self.buffer.len() > 0 && self.insert_index > 0 {
                        self.insert_index -= 1;
                        self.buffer.remove(self.insert_index);
                        self.guesser.process(self.buffer.iter().collect());
                    }
                }
                Key::Delete => {
                    if self.buffer.len() > 0 && self.insert_index < self.buffer.len() {
                        self.buffer.remove(self.insert_index);
                        self.guesser.process(self.buffer.iter().collect());
                    }
                }
                Key::Enter => {
                    self.buffer.push('\n');
                    self.terminal.write_new_line();
                    return true;
                }
                Key::Esc | Key::Ctrl('c') => {
                    self.reset();
                }
                Key::Ctrl('l') => {
                    self.reset();
                    self.terminal.clear();
                }
                Key::Tab => {
                    let opt = self.guesser.fill_with_next();
                    if let Some(guess) = opt {
                        self.buffer = guess;
                        self.insert_index = self.buffer.len();
                        if self.guesser.count() == 1 {
                            self.guesser.process(self.buffer.iter().collect());
                        }
                    }
                }
                _ => {
                    return false;
                }
            }
        }
    }

    pub fn new(
        keyboard: Box<Keyboard>,
        terminal: Box<Terminal>,
        guesser: GuesserManager,
        interpreter: Interpreter,
        context: Rc<Context>,
    ) -> ShellController {
        return ShellController {
            buffer: Vec::new(),
            insert_index: 0,
            keyboard,
            terminal,
            guesser,
            interpreter,
            context,
        };
    }
}
