use std::fs::File;
use std::io::Write;
use std::io::BufReader;
use std::io::Lines;
use std::io::BufRead;
use std::io;

pub struct UserHistory {
    file: File
}
impl UserHistory {
    pub fn new(file: File) -> UserHistory {
        return UserHistory{
            file,
        }
    }

    pub fn record(&mut self, input: &str) {
        write!(self.file, "{}", input);
    }

    pub fn iter(&self) -> UserHistoryIterator  {
        let tmp = self.file.try_clone().expect("couldn't copy file");
        let mut buf = BufReader::new(tmp);
        return UserHistoryIterator{
            lines: buf.lines()
        }
    }
}

pub struct UserHistoryIterator {
    lines: Lines<BufReader<File>>
}
impl Iterator for UserHistoryIterator {
    type Item = io::Result<String>;

    fn next(&mut self) -> Option<io::Result<String>> {
        return self.lines.next();
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn record() {
        let mut history = UserHistory::new(fs::OpenOptions::new()
            .write(true)
            .truncate(true)
            .create(true)
            .open("/tmp/record_test.txt")
            .expect("Couldn't open test file"));
        history.record("ls");
    }
}

