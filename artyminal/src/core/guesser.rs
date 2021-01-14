pub struct GuessRequest {
    pub user_input: String,
    pub current_dir: std::path::PathBuf,
}
impl GuessRequest {
    pub fn new(user_input: String, current_dir: std::path::PathBuf) -> GuessRequest {
        return GuessRequest {
            user_input,
            current_dir,
        };
    }
}

pub trait Guesser {
    fn guess(&self, request: &GuessRequest) -> Vec<Guess>;
}

#[derive(Clone)]
pub struct Guess {
    guess: String,
    missing: String,
}

impl Guess {
    pub fn new(guess: String, missing: String) -> Guess {
        return Guess { guess, missing };
    }

    pub fn as_string(&self) -> &String {
        return &self.guess;
    }

    pub fn missing(&self) -> &String {
        return &self.missing;
    }
}

pub struct GuesserManager {
    workers: Vec<Box<dyn Guesser>>,
    guesses: Vec<Guess>,
    index: usize,
    user_input: String,
}
impl GuesserManager {
    pub fn new() -> GuesserManager {
        return GuesserManager {
            workers: Vec::new(),
            guesses: Vec::new(),
            index: 0,
            user_input: "".to_string(),
        };
    }

    pub fn add(&mut self, guesser: Box<dyn Guesser>) {
        self.workers.push(guesser);
    }

    pub fn to_string_vec(&self) -> Vec<String> {
        let mut res = Vec::new();
        for guess in self.guesses.iter() {
            res.push(guess.as_string().clone());
        }
        return res;
    }

    pub fn next(&mut self) -> Option<Guess> {
        if self.guesses.len() == 0 {
            return None;
        }
        let res = Some(self.guesses[self.index % self.guesses.len()].clone());
        self.index += 1;
        return res;
    }

    pub fn fill_with_next(&mut self) -> Option<Vec<char>> {
        let mut tmp = self.user_input.clone();
        if let Some(guess) = self.next() {
            tmp.push_str(guess.missing().as_str());
            return Some(tmp.chars().collect());
        } else {
            return None;
        }
    }

    pub fn process(&mut self, request: &GuessRequest) {
        self.guesses = Vec::new();
        self.user_input = request.user_input.clone();
        if self.user_input.len() == 0 {
            return;
        }
        for worker in self.workers.iter() {
            let mut tmp = worker.guess(request);
            self.guesses.append(&mut tmp);
        }
    }

    pub fn count(&self) -> usize {
        return self.guesses.len();
    }
}
