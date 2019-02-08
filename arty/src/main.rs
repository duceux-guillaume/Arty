extern crate arty;

use std::rc::Rc;

use arty::core::guesser::GuesserManager;
use arty::feature::guesser::PathGuesser;
use arty::feature::guesser::FileGuesser;
use arty::external::terminal_termion_impl::TermionKeyboard;
use arty::external::terminal_termion_impl::TermionTerminal;
use arty::feature::shell::{ShellController, Context};
use arty::feature::interpreter::Interpreter;

fn main() {
    println!("Hello World");
    let terminal = Box::new(TermionTerminal::new());
    let keyboard = Box::new(TermionKeyboard::new());
    let mut guesser = GuesserManager::new();
    let context = Rc::new(Context::new());
//    guesser.add(Box::new(FileGuesser::new(Rc::clone(&context))));
    guesser.add(Box::new(PathGuesser::new(Rc::clone(&context))));
    let mut shell = ShellController::new(keyboard, terminal, guesser, Interpreter::new(), context);
    shell.run();
}
