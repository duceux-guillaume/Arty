pub trait Guesser {
    fn guess(&self, request: String) -> Vec<Guess>;
}

#[derive(Clone)]
pub struct Guess {
    line: String,
}

impl Guess {
    pub fn new(line: String) -> Guess {
        return Guess { line };
    }

    pub fn as_string(&self) -> String {
        return self.line.clone();
    }

    pub fn as_char(&self) -> Vec<char> {
        return self.line.chars().collect();
    }
}

pub struct GuesserManager {
    workers: Vec<Box<Guesser>>,
    guesses: Vec<Guess>,
    index: usize,
}
impl GuesserManager {
    pub fn new() -> GuesserManager {
        return GuesserManager {
            workers: Vec::new(),
            guesses: Vec::new(),
            index: 0,
        };
    }

    pub fn add(&mut self, guesser: Box<Guesser>) {
        self.workers.push(guesser);
    }

    pub fn to_string_vec(&self) -> Vec<String> {
        let mut res = Vec::new();
        for guess in self.guesses.iter() {
            res.push(guess.as_string());
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

    pub fn process(&mut self, request: String) {
        self.guesses = Vec::new();
        if request.len() == 0 {
            return;
        }
        for worker in self.workers.iter() {
            let mut tmp = worker.guess(request.clone());
            self.guesses.append(&mut tmp);
        }
    }
}
