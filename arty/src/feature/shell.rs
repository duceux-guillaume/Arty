use core::guesser::{GuessRequest, GuesserManager};
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
    context: Context,
    in_guess_selection: bool,
}
impl ShellController {
    fn reset(&mut self) {
        self.buffer = Vec::new();
        self.insert_index = 0;
        self.guess();
        self.in_guess_selection = false;
    }

    pub fn run(&mut self) {
        loop {
            self.reset();
            if !self.read_user_input() {
                println!("bye bye");
                return;
            }
            self.interpreter.process(self.buffer.iter().collect(), &mut self.context);
        }
    }

    fn guess(&mut self) {
        self.guesser.process(&GuessRequest::new(self.buffer.iter().collect(),
                                                self.context.current_directory().clone()));
    }

    fn read_user_input(&mut self) -> bool {
        loop {
            self.terminal.write_guesses(self.guesser.to_string_vec());
            self.terminal.write_cursor(self.insert_index);
            self.terminal.write_line(self.buffer.iter().collect());
            match self.keyboard.get_key() {
                Key::Char(charr) => {
                    if charr != ' ' || !self.in_guess_selection {
                        self.buffer.insert(self.insert_index, charr);
                        self.insert_index += 1;
                    } else {
                        self.in_guess_selection = false;
                    }
                    self.guess();
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
                        self.guess();
                    }
                }
                Key::Delete => {
                    if self.buffer.len() > 0 && self.insert_index < self.buffer.len() {
                        self.buffer.remove(self.insert_index);
                        self.guess();
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
                            self.guess();
                        } else {
                            self.in_guess_selection = true;
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
    ) -> ShellController {
        return ShellController {
            buffer: Vec::new(),
            insert_index: 0,
            keyboard,
            terminal,
            guesser,
            interpreter,
            context: Context::new(),
            in_guess_selection: false,
        };
    }
}
