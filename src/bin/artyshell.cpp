#include <iostream>

#include <arty/core/number.h>
#include <arty/impl/lexer.h>

using namespace arty;

int main(int argc, char** argv) {
    std::string input;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            input.append(argv[i]);
            input.append(" ");
        }
    }

    std::cout << input << std::endl;

    Lexer lex(input);
    Token t;
    do {
        t = lex.next();
    } while (t.type != Eof && t.type != Error);
    std::cout << lex << std::endl;

    return 0;
}
