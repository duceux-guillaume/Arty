use core::guesser::{GuessRequest, GuesserManager};
use core::terminal::Key;
use core::terminal::Keyboard;
use core::terminal::Terminal;
use core::user_history::UserSessionHistory;

use feature::interpreter::Interpreter;

pub struct Context {
    current_directory: std::path::PathBuf,
}
impl Context {
    pub fn new() -> Context {
        return Context {
            current_directory: std::env::current_dir().unwrap(),
        };
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
    keyboard: Box<dyn Keyboard>,
    terminal: Box<dyn Terminal>,
    guesser: GuesserManager,
    interpreter: Interpreter,
    context: Context,
    in_guess_selection: bool,
    session_history: UserSessionHistory,
    session_index: usize,
}
impl ShellController {
    fn reset(&mut self) {
        self.buffer = Vec::new();
        self.insert_index = 0;
        self.guess();
        self.in_guess_selection = false;
        self.session_index = 0;
    }

    pub fn run(&mut self) {
        loop {
            self.reset();
            if !self.read_user_input() {
                println!("bye bye");
                return;
            }
            self.interpreter
                .process(self.buffer.iter().collect(), &mut self.context);
            self.session_history.record(self.buffer.iter().collect());
        }
    }

    fn guess(&mut self) {
        self.guesser.process(&GuessRequest::new(
            self.buffer.iter().collect(),
            self.context.current_directory().clone(),
        ));
    }

    fn compute_prompt(&self) -> String {
        return format!(
            "{}@{}{}",
            "\x1B[38;5;1m",
            self.context
                .current_directory()
                .file_name()
                .unwrap()
                .to_str()
                .unwrap(),
            "\x1B[38;5;15m"
        );
    }

    fn read_user_input(&mut self) -> bool {
        loop {
            self.terminal.write_guesses(self.guesser.to_string_vec());
            self.terminal.write_cursor(self.insert_index);
            let prompt = self.compute_prompt();
            self.terminal.write_prompt(prompt);
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
                    self.terminal.clear_guesses();
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
                Key::Up => {
                    let history_size = self.session_history.entries().len();
                    if history_size > 0 {
                        self.session_index += 1;
                        if self.session_index > history_size {
                            self.session_index = 1;
                        }
                        let index = history_size - self.session_index;
                        self.buffer = self.session_history.entries()[index]
                            .trim()
                            .chars()
                            .collect();
                        self.insert_index = self.buffer.len();
                    }
                }
                Key::Down => {
                    let history_size = self.session_history.entries().len();
                    if history_size > 0 {
                        if self.session_index > 1 {
                            self.session_index -= 1;
                            let index = history_size - self.session_index;
                            self.buffer = self.session_history.entries()[index]
                                .trim()
                                .chars()
                                .collect();
                            self.insert_index = self.buffer.len();
                        } else {
                            self.reset();
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
        keyboard: Box<dyn Keyboard>,
        terminal: Box<dyn Terminal>,
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
            session_history: UserSessionHistory::new(),
            session_index: 0,
        };
    }
}
