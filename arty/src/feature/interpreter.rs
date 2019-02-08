use std::rc::Rc;

use parser::Parser;
use super::shell::Context;

pub struct Interpreter {}
impl Interpreter {
    pub fn new() -> Interpreter {
        return Interpreter {};
    }

    pub fn process(&mut self, line: String, ctx: Rc<Context>) {
        let res = Parser::process(line.clone(), ctx);
        match res {
            Ok(ref str) => {
                if !str.as_string().is_empty() {
                    println!("{}", str.as_string())
                }
            }
            Err(ref str) => println!("{}", str),
        }
    }
}
