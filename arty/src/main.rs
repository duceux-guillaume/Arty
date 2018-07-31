extern crate arty;

use arty::parser;
use arty::parser::ShellContext;

fn main() {
    println!("Hello, world!");

    let mut ctx= match ShellContext::new() {
        Ok(shell_ctx) => shell_ctx,
        _ => panic!("no env"),
    };
    loop {
        let mut line = String::new();
        std::io::stdin().read_line(&mut line)
            .expect("Failed to read line");

        let res = parser::Parser::process(line, &mut ctx);
        println!("------");
        match res {
            Ok(ref str) => println!("Sucess: {}", str),
            Err(ref str) => println!("error: {}", str)
        }
        println!("------");
    }
}
