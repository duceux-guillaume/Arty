extern crate arty;

use arty::parser;

fn main() {
    println!("Hello, world!");

    loop {
        let mut line = String::new();
        std::io::stdin().read_line(&mut line)
            .expect("Failed to read line");

        let res = parser::Parser::process(line);
        println!("------");
        match res {
            Ok(ref str) => println!("Sucess: {}", str),
            Err(ref str) => println!("error: {}", str)
        }
        println!("------");
    }
}
