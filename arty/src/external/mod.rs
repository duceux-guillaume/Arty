extern crate dirs;

use std::fs;

pub struct UserHistoryFileCreator {}
impl UserHistoryFileCreator {
    pub fn create_default_file() -> fs::File {
        let mut history_path = dirs::home_dir().expect("Couldn't get home directory");
        history_path.push(".arty");
        if !history_path.exists() {
            fs::create_dir(history_path.as_path()).expect(&format!("Couldn't create {:?}", history_path));
        }
        history_path.push("history.txt");
        let mut file = fs::OpenOptions::new()
            .write(true)
            .append(true)
            .create(true)
            .open(history_path.as_path())
            .expect(&format!("Failed to create {:?}", history_path));
        return file;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn create_default_file() {
        UserHistoryFileCreator::create_default_file();
    }
}
