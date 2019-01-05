use core::terminal::Key;
use core::terminal::Keyboard;
use core::terminal::Terminal;

pub struct ShellController {
    buffer: Vec<char>,
    keyboard: Box<Keyboard>,
    terminal: Box<Terminal>,
}
impl ShellController {
    pub fn run(&mut self) {
        loop {
            match self.keyboard.get_key() {
                Key::Char(charr) => {
                        self.buffer.push(charr);
                }
                _ => {}
            }
        }
    }
}

