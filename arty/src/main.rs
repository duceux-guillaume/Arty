extern crate arty;

use arty::core::guesser::DummyGuesser;
use arty::core::guesser::GuesserManager;
use arty::core::guesser::ZeroGuesser;
use arty::external::terminal_termion_impl::TermionKeyboard;
use arty::external::terminal_termion_impl::TermionTerminal;
use arty::feature::shell::ShellController;
use arty::feature::interpreter::Interpreter;

fn main() {
    println!("Hello World");
    let terminal = Box::new(TermionTerminal::new());
    let keyboard = Box::new(TermionKeyboard::new());
    let mut guesser = GuesserManager::new();
    guesser.add(Box::new(DummyGuesser::new()));
    guesser.add(Box::new(ZeroGuesser::new()));
    let mut shell = ShellController::new(keyboard, terminal, guesser, Interpreter::new());
    shell.run();
}
