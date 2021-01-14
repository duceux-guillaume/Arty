extern crate artyminal;

use artyminal::core::guesser::GuesserManager;
use artyminal::external::terminal_impl::KeyboardImpl;
use artyminal::external::terminal_impl::TerminalImpl;
use artyminal::feature::guesser::FileGuesser;
use artyminal::feature::guesser::PathGuesser;
use artyminal::feature::interpreter::Interpreter;
use artyminal::feature::shell::ShellController;

fn main() {
    println!("Hello World");
    let terminal = Box::new(TerminalImpl::new());
    let keyboard = Box::new(KeyboardImpl::new());
    let mut guesser = GuesserManager::new();
    guesser.add(Box::new(FileGuesser::new()));
    guesser.add(Box::new(PathGuesser::new()));
    let mut shell = ShellController::new(keyboard, terminal, guesser, Interpreter::new());
    shell.run();
}
