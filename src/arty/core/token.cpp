#include <arty/core/token.h>

namespace arty {

std::ostream &operator<<(std::ostream &out, const Token &t) {
    return out << '(' << t.symbol << ')';
}

Token Token::Eof(std::size_t pos) {
    return Token(TokenType::Eof, "\n", pos);
}

Token Token::None(std::size_t pos) {
    return Token(TokenType::None, " ", pos);
}

}  // arty
