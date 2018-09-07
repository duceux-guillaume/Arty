use std::env;
use std::path::PathBuf;

use language::Token;

use parser::ShellContext;

use lexer::Lexer;

use filesystem::SearchFor;

pub struct Guess {
    first_part: Vec<char>,
    missing_part: Vec<char>
}
impl Guess {
    pub fn new(first: &str, second: &str) -> Guess {
        return Guess {
            first_part: first.chars().collect(),
            missing_part: second.chars().collect(),
        }
    }

    pub fn missing_part(&self) -> Vec<char> {
        return  self.missing_part.clone();
    }

    pub fn to_string(&self) -> String {
        let first: String = self.first_part.iter().collect();
        let second: String = self.missing_part.iter().collect();
        return format!("{}{}", first, second)
    }
}

pub struct PathGuesser {}
impl PathGuesser {
    pub fn new() -> PathGuesser {
        return PathGuesser{}
    }

    pub fn guess(&self, ctx: &ShellContext, line: &Vec<char>) -> Vec<Guess> {
        let mut lexer = Lexer::new(line.iter().collect());
        let mut result = Vec::new();
        // Check first token
        if let Token::ChangeDir = lexer.get(0) {
            // Then second
            match lexer.get(1) {
                Token::None | Token::Eof => {
                    result = self.convert(self.list_directory(&ctx.env));
                },
                Token::Path(hint) => {
                    // we have arguments, let's figure out where the user went
                    let path_hint = PathBuf::from(hint.clone());
                    if !path_hint.is_absolute() {
                        let mut env = ctx.env.clone();
                        env.push(path_hint.clone());
                        result = self.guess_directory_from_hint(env);
                    } else {
                        result = self.guess_directory_from_hint(path_hint);
                    }
                },
                _ => {
                    println!("what?")
                },
            }
        }
        return result
    }

    fn guess_directory_from_hint(&self, hint: PathBuf) -> Vec<Guess> {
        return if hint.exists() {
            self.convert(self.list_directory(&hint))
        } else {
            // no arguments yet, return context directory
            let missing_hint = hint.file_name().unwrap().to_str().unwrap().to_string();
            self.filter_match(&missing_hint, self.list_partial_directory(&hint))
        }
    }

    fn list_directory(&self, ctx: &PathBuf) -> Vec<String> {
        let mut result = Vec::new();
        if ctx.is_dir() {
            for entry in ctx.read_dir().expect("can't list dir") {
                let path = entry.expect("whatever").path();
                if path.is_dir() {
                    let mut path_name = String::from(path.file_name().unwrap().to_str().unwrap());
                    path_name.push('/');
                    result.push(path_name);
                }
            }
        }
        return result
    }

    fn list_partial_directory(&self, ctx: &PathBuf) -> Vec<String> {
        return if ctx.exists() {
            self.list_directory(ctx)
        } else {
            self.list_directory(&ctx.parent().unwrap().to_path_buf())
        }
    }

    fn filter_match(&self, pattern: &String, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            if let Some(guess) = self.guess_from_match(item, &pattern.to_string()) {
                result.push(guess)
            }
        }
        return result
    }

    fn convert(&self, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            result.push(Guess::new("", item.as_str()));
        }
        return result
    }

    fn guess_from_match(&self, entry: &String, pattern: &String) -> Option<Guess> {
        let matching = entry.get(0..pattern.len());
        if matching.is_none() {
            return None
        }
        if matching.unwrap() != pattern {
            return None
        }
        let result = entry.get(pattern.len()..entry.len());
        if result.is_some() {
            return Some(Guess::new(
                pattern.as_str(),
                result.unwrap(),
            ))
        }
        return None
    }
}

pub struct FileGuesser {}
impl FileGuesser {
    pub fn new() -> FileGuesser {
        return FileGuesser {}
    }

    pub fn guess(&self, ctx: &ShellContext, line: &Vec<char>) -> Vec<Guess> {
        let mut lexer = Lexer::new(line.iter().collect());
        let mut result = Vec::new();
        // Check first token
        let token = lexer.get(0);
        if let Token::Cmd(_cmd) = token {
            // Then second
            match lexer.get(1) {
                Token::None | Token::Eof => {
                    result = self.convert(self.list_files(&ctx.env));
                },
                Token::CmdArgs(hint) => {
                    // we have arguments, let's figure out where the user went
                    let path_hint = PathBuf::from(hint.clone());
                    if !path_hint.is_absolute() {
                        let mut env = ctx.env.clone();
                        env.push(path_hint.clone());
                        result = self.guess_file_from_hint(env);
                    } else {
                        result = self.guess_file_from_hint(path_hint);
                    }
                },
                _ => {
                    println!("what?")
                },
            }
        } else {
            let paths = SearchFor::starting_with(token.as_string()).in_path(&ctx.env);
            for item in paths.iter() {
                let file_name = item.file_name().unwrap().to_str().unwrap().to_string();
                let option = self.guess_from_match(&file_name, &token.as_string());
                if option.is_some() {
                    result.push(option.unwrap());
                }
            }
        }
        return result
    }

    fn list_files(&self, ctx: &PathBuf) -> Vec<String> {
        let mut result = Vec::new();
        if ctx.is_dir() {
            for entry in ctx.read_dir().expect("can't list dir") {
                let file = entry.expect("whatever").path();
                let mut file_name = String::from(file.file_name().unwrap().to_str().unwrap());
                if file.is_dir() {
                    file_name.push('/');
                }
                result.push(file_name);
            }
        }
        return result
    }

    fn convert(&self, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            result.push(Guess::new("", item.as_str()));
        }
        return result
    }


    fn guess_file_from_hint(&self, hint: PathBuf) -> Vec<Guess> {
        return if hint.exists() {
            self.convert(self.list_files(&hint))
        } else {
            // no arguments yet, return context directory
            let missing_hint = hint.file_name().unwrap().to_str().unwrap().to_string();
            self.filter_match(&missing_hint, self.list_partial_files(&hint))
        }
    }

    fn list_partial_files(&self, ctx: &PathBuf) -> Vec<String> {
        return if ctx.exists() {
            self.list_files(ctx)
        } else {
            self.list_files(&ctx.parent().unwrap().to_path_buf())
        }
    }

    fn filter_match(&self, pattern: &String, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            if let Some(guess) = self.guess_from_match(item, &pattern.to_string()) {
                result.push(guess)
            }
        }
        return result
    }

    fn guess_from_match(&self, entry: &String, pattern: &String) -> Option<Guess> {
        let matching = entry.get(0..pattern.len());
        if matching.is_none() {
            return None
        }
        if matching.unwrap() != pattern {
            return None
        }
        let result = entry.get(pattern.len()..entry.len());
        if result.is_some() {
            return Some(Guess::new(
                pattern.as_str(),
                result.unwrap(),
            ))
        }
        return None
    }
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn guess_from_good_match() {
        let guesser = PathGuesser::new();
        let guess = guesser.guess_from_match(&"target/".to_string(),
                                             &"t".to_string());
        assert!(guess.is_some());
        let missing_part: String = guess.unwrap().missing_part().iter().collect();
        assert_eq!("arget/", missing_part);
    }

    #[test]
    fn guess_from_bad_match() {
        let guesser = PathGuesser::new();
        let guess = guesser.guess_from_match(&"target/".to_string(),
                                             &"what".to_string());
        assert!(guess.is_none());
    }

}