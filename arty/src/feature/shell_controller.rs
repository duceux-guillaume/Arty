use core::terminal::Key;
use core::terminal::Keyboard;
use core::terminal::Terminal;

pub struct ShellController {
    buffer: Vec<char>,
    insert_index: usize,
    keyboard: Box<Keyboard>,
    terminal: Box<Terminal>,
}
impl ShellController {
    pub fn run(&mut self) {
        loop {
            self.terminal.write_cursor(self.insert_index);
            self.terminal.write_line(self.buffer.iter().collect());
            match self.keyboard.get_key() {
                Key::Char(charr) => {
                        self.buffer.insert(self.insert_index, charr);
                        self.insert_index += 1;
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
                    }
                }
                Key::Delete => {
                    if self.buffer.len() > 0 && self.insert_index < self.buffer.len() {
                        self.buffer.remove(self.insert_index);
                    }
                }
                Key::Enter => {
                    self.buffer.push('\n');
                    self.terminal.write_new_line();
                }
                _ => {
                    return;
                }
            }
        }
    }

    pub fn new(keyboard: Box<Keyboard>, terminal: Box<Terminal>) -> ShellController {
        return ShellController {
            buffer: Vec::new(),
            insert_index: 0,
            keyboard,
            terminal,
        }
    }
}

