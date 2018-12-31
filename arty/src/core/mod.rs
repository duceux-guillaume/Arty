use std::fs::File;
use std::io::Write;
use std::io::BufReader;
use std::io::Lines;
use std::io::BufRead;
use std::io::Seek;
use std::io;

pub struct UserHistoryFile {
    file: File
}
impl UserHistoryFile {
    pub fn new(file: File) -> UserHistoryFile {
        return UserHistoryFile {
            file,
        }
    }

    pub fn record(&mut self, input: &str) {
        write!(self.file, "{}", input);
    }

    pub fn iter(&self) -> UserHistoryIterator  {
        let mut tmp = self.file.try_clone().expect("couldn't clone file");
        tmp.seek(io::SeekFrom::Start(0)).expect("couldn't seek to 0");
        let buf = BufReader::new(tmp);
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

pub struct UserHistorySearch {
    iterator: UserHistoryIterator
}
impl UserHistorySearch {
    fn new(iterator: UserHistoryIterator) -> UserHistorySearch {
        return UserHistorySearch {
            iterator,
        }
    }

    fn search(&mut self, pat: &str) -> Vec<String> {
        let mut res = Vec::new();
        while let Some(Ok(line)) = self.iterator.next() {
            if line.find(pat).is_some() {
                res.push(line);
            }
        }
        return res;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    #[test]
    fn record() {
        let file = fs::OpenOptions::new()
            .write(true)
            .truncate(true)
            .create(true)
            .open("/tmp/record_test.txt")
            .expect("Couldn't open test file");
        let mut history = UserHistoryFile::new(file);
        history.record("ls\n");
        let file2 = fs::OpenOptions::new()
            .read(true)
            .open("/tmp/record_test.txt")
            .expect("Couldn't open test file");
        let mut lines = BufReader::new(file2).lines().map(|l| l.unwrap());
        assert_eq!(Some(String::from("ls")), lines.next());
    }

    #[test]
    fn test() {
        let mut history = UserHistoryFile::new(fs::OpenOptions::new()
            .write(true)
            .read(true)
            .truncate(true)
            .create(true)
            .open("/tmp/iter_test.txt")
            .expect("Couldn't open test file"));
        history.record("1\n");
        history.record("2\n");
        let mut it = history.iter().map(|l| l.unwrap());
        assert_eq!(Some(String::from("1")), it.next());
        assert_eq!(Some(String::from("2")), it.next());
        assert_eq!(None, it.next());
        history.record("3\n");
        assert_eq!(None, it.next());
    }

    #[test]
    fn search() {
        let mut history = UserHistoryFile::new(fs::OpenOptions::new()
            .write(true)
            .read(true)
            .truncate(true)
            .create(true)
            .open("/tmp/search_test.txt")
            .expect("Couldn't open test file"));
        history.record("toto\n");
        history.record("tata\n");
        let mut re = UserHistorySearch::new(history.iter());
        assert_eq!(vec![String::from("toto"), String::from("tata")], re.search("t"));
        re = UserHistorySearch::new(history.iter());
        assert_eq!(vec![String::from("tata")], re.search("ta"));
    }
}

