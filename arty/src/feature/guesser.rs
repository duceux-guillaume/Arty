use std::path::PathBuf;
use std::rc::Rc;
use language::Token;
use super::shell::Context;
use lexer::Lexer;
use filesystem::SearchFor;
use core::guesser::{Guess, Guesser, GuessRequest};

pub struct PathGuesser {}
impl PathGuesser {
    pub fn new() -> PathGuesser {
        return PathGuesser {}
    }

    fn guess_directory_from_hint(&self, hint: PathBuf) -> Vec<Guess> {
        return if hint.exists() {
            self.convert(self.list_directory(&hint))
        } else {
            // no arguments yet, return context directory
            let missing_hint = hint.file_name().unwrap().to_str().unwrap().to_string();
            self.filter_match(&missing_hint, self.list_partial_directory(&hint))
        };
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
        return result;
    }

    fn list_partial_directory(&self, ctx: &PathBuf) -> Vec<String> {
        return if ctx.exists() {
            self.list_directory(ctx)
        } else {
            self.list_directory(&ctx.parent().unwrap().to_path_buf())
        };
    }

    fn filter_match(&self, pattern: &String, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            if let Some(guess) = self.guess_from_match(item, &pattern.to_string()) {
                result.push(guess)
            }
        }
        return result;
    }

    fn convert(&self, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            result.push(Guess::new(item.to_string(), item.to_string()));
        }
        return result;
    }

    fn guess_from_match(&self, entry: &String, pattern: &String) -> Option<Guess> {
        let matching = entry.get(0..pattern.len());
        if matching.is_none() {
            return None;
        }
        if matching.unwrap() != pattern {
            return None;
        }
        let result = entry.get(pattern.len()..entry.len());
        if let Some(missing) = result {
            return Some(Guess::new(entry.to_string(), missing.to_string()));
        }
        return None;
    }
}
impl Guesser for PathGuesser {
    fn guess(&self, request: &GuessRequest) -> Vec<Guess> {
        let mut lexer = Lexer::new(request.user_input.clone());
        let mut result = Vec::new();
        // Check first token
        if let Token::ChangeDir = lexer.get(0) {
            // Then second
            match lexer.get(1) {
                Token::None | Token::Eof => {
                    result = self.convert(self.list_directory(&request.current_dir));
                }
                Token::Path(hint) => {
                    // we have arguments, let's figure out where the user went
                    let path_hint = PathBuf::from(hint.clone());
                    if !path_hint.is_absolute() {
                        let mut env = request.current_dir.clone();
                        env.push(path_hint.clone());
                        result = self.guess_directory_from_hint(env);
                    } else {
                        result = self.guess_directory_from_hint(path_hint);
                    }
                }
                _ => println!("what?"),
            }
        }
        return result;
    }
}

pub struct FileGuesser {}
impl FileGuesser {
    pub fn new() -> FileGuesser {
        return FileGuesser {};
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
        return result;
    }

    fn convert(&self, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            result.push(Guess::new(item.to_string(), item.to_string()));
        }
        return result;
    }

    fn guess_file_from_hint(&self, hint: PathBuf) -> Vec<Guess> {
        return if hint.exists() {
            self.convert(self.list_files(&hint))
        } else {
            // no arguments yet, return context directory
            let missing_hint = hint.file_name().unwrap().to_str().unwrap().to_string();
            self.filter_match(&missing_hint, self.list_partial_files(&hint))
        };
    }

    fn list_partial_files(&self, ctx: &PathBuf) -> Vec<String> {
        return if ctx.exists() {
            self.list_files(ctx)
        } else {
            self.list_files(&ctx.parent().unwrap().to_path_buf())
        };
    }

    fn filter_match(&self, pattern: &String, src: Vec<String>) -> Vec<Guess> {
        let mut result = Vec::new();
        for item in src.iter() {
            if let Some(guess) = self.guess_from_match(item, &pattern.to_string()) {
                result.push(guess)
            }
        }
        return result;
    }

    fn guess_from_match(&self, entry: &String, pattern: &String) -> Option<Guess> {
        let matching = entry.get(0..pattern.len());
        if matching.is_none() {
            return None;
        }
        if matching.unwrap() != pattern {
            return None;
        }
        let result = entry.get(pattern.len()..entry.len());
        if let Some(missing) = result {
            return Some(Guess::new(entry.to_string(), missing.to_string()));
        }
        return None;
    }
}
impl Guesser for FileGuesser {
    fn guess(&self, request: &GuessRequest) -> Vec<Guess> {
        let mut lexer = Lexer::new(request.user_input.clone());
        let mut result = Vec::new();
        // Check first token
        let token = lexer.get(0);
        if let Token::ChangeDir = token {
            return result;
        }
        if let Token::Cmd(_cmd) = token {
            // Then second
            match lexer.get(1) {
                Token::None | Token::Eof => {
                    result = self.convert(self.list_files(&request.current_dir));
                }
                Token::CmdArgs(hint) => {
                    // we have arguments, let's figure out where the user went
                    let path_hint = PathBuf::from(hint.clone());
                    if !path_hint.is_absolute() {
                        let mut env = request.current_dir.clone();
                        env.push(path_hint.clone());
                        result = self.guess_file_from_hint(env);
                    } else {
                        result = self.guess_file_from_hint(path_hint);
                    }
                }
                _ => println!("what?"),
            }
        } else {
            let paths = SearchFor::starting_with(token.as_string()).in_path(&request.current_dir);
            for item in paths.iter() {
                let file_name = item.file_name().unwrap().to_str().unwrap().to_string();
                let option = self.guess_from_match(&file_name, &token.as_string());
                if option.is_some() {
                    result.push(option.unwrap());
                }
            }
        }
        return result;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn guess_from_good_match() {
        let guesser = PathGuesser::new();
        let guess = guesser.guess_from_match(&"target/".to_string(), &"t".to_string());
        assert!(guess.is_some());
        let missing_part: String = guess.unwrap().missing_part().iter().collect();
        assert_eq!("arget/", missing_part);
    }

    #[test]
    fn guess_from_bad_match() {
        let guesser = PathGuesser::new();
        let guess = guesser.guess_from_match(&"target/".to_string(), &"what".to_string());
        assert!(guess.is_none());
    }

}
