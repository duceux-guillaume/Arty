

fn main() {
    println!("Hello, world!");

    let mut line = String::new();
    std::io::stdin().read_line(&mut line)
        .expect("Failed to read line");

    println!("{}", line);
    for c in line.bytes() {
        println!("{}", c);
    }
    for c in line.chars() {
        println!("{}", c);
    }
}
