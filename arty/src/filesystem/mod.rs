use std::path::PathBuf;

enum MatchingPattern {
    Exact(String),
    Start(String),
}
impl MatchingPattern {
    fn match_with(&self, path: &String) -> bool {
        return match *self {
            MatchingPattern::Exact(ref string) => MatchingPattern::exact_match(string, path),
            MatchingPattern::Start(ref string) => MatchingPattern::start_match(string, path),
        }
    }

    fn exact_match(pat: &String, filename: &String) -> bool {
        return pat.eq(filename)
    }

    fn start_match(pat: &String, filename: &String) -> bool {
        if pat.len() > filename.len() {
            return false;
        }
        return pat.eq(filename.get(0..pat.len()).unwrap())
    }
}

pub struct SearchTask {
    pattern: MatchingPattern
}

impl SearchTask {
    pub fn in_paths(&self, paths: Vec<PathBuf>) -> Vec<PathBuf> {
        let mut res = Vec::new();
        for path in paths.iter() {
            if path.is_dir() {
                self.search_directory(path, &mut res);
            }
        }
        return res;
    }

    pub fn in_path(&self, path: &PathBuf) -> Vec<PathBuf> {
        let mut res = Vec::new();
        if path.is_dir() {
            self.search_directory(&path, &mut res);
        }
        return res;
    }


    fn search_directory(&self, path: &PathBuf, res: &mut Vec<PathBuf>) {
        for item in path.read_dir().expect("can't read directory") {
            let item_path = item.unwrap();
            let file_name = item_path.file_name().into_string().unwrap();
            if self.pattern.match_with(&file_name) {
                res.push(item_path.path());
            }
        }
    }
}

pub struct SearchFor {}

impl SearchFor {
    pub fn matching(name: String) -> SearchTask {
        return SearchTask {
            pattern: MatchingPattern::Exact(name),
        };
    }

    pub fn starting_with(name: String) -> SearchTask {
        return SearchTask {
            pattern: MatchingPattern::Start(name),
        };
    }
}
