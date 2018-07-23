extern crate arty;

use arty::lexer;

fn main() {
    println!("Hello, world!");

    let mut line = String::new();
    std::io::stdin().read_line(&mut line)
        .expect("Failed to read line");

    let mut lexer = lexer::Lexer::new();
    let res = lexer.process(line);
    println!("------");
    for l in res.iter() {
        println!("{}", l);
    }
    println!("------");
}
