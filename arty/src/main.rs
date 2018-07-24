extern crate arty;

use arty::parser;

fn main() {
    println!("Hello, world!");

    loop {
        let mut line = String::new();
        std::io::stdin().read_line(&mut line)
            .expect("Failed to read line");

        let res: String = parser::Parser::process(line);
        println!("------");
        println!("{}", res);
        println!("------");
    }
}
