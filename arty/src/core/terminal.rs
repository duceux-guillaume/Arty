pub trait Terminal {
    fn clear(&self);
    fn write_prompt(&self, line: String);
    fn write_line(&self, line: String);
    fn write_guesses(&self, line: Vec<String>);
    fn write_new_line(&self);
}

pub enum Key {
    Char(char),
    Up,
    Left,
    Down,
    Right,
    Ctrl(char),
    Alt(char),
    Esc,
    Backspace,
    Delete,
    Tab,
    Enter,
}

pub trait Keyboard {
    fn get_key(&self) -> Key;
}
