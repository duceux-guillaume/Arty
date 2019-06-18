#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

namespace arty {

enum TokenType {
    Error,
    Eof,
    None,
    // Math
    Number,
    Plus,
    Minus,
    Times,
    Divide,
    // Shell
    Cmd,
    Opt,
    // Marks
    Semi,
    Amp,
    Pipe,
    // BuiltIn
    Calc,
};

struct Token {
    Token(TokenType t, std::string const& s, std::size_t pos):
        type(t),
        symbol(s),
        first_char_pos(pos),
        last_char_pos(pos + symbol.size())
    {}
    Token(): Token(TokenType::None, "", 0) {}

    TokenType type;
    std::string symbol;
    std::size_t first_char_pos;
    std::size_t last_char_pos;

    static Token Eof(std::size_t pos);
    static Token None(std::size_t pos);

    std::size_t length() const { return last_char_pos - first_char_pos; }
};

std::ostream& operator<<(std::ostream& out, Token const& t);

}  // arty

#endif // TOKEN_H
