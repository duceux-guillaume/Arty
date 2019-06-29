#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

namespace arty {

enum TokenType {
  Error,
  Eof,
  None,
  // Math
  TkNumber,
  Plus,
  Minus,
  Times,
  Divide,
  TkParL,
  TkParR,
  // Shell
  Cmd,
  Opt,
  // Marks
  Semi,
  Amp,
  Pipe,
  // BuiltIn Cmd
  Calc,
  TkCd,
  TkVar,
  // Control flow
  TkIf,
  TkFor,
  TkWhile,
};

struct Token {
  Token(TokenType t, std::string s, std::size_t pos)
      : type(t),
        symbol(std::move(s)),
        first_char_pos(pos),
        last_char_pos(pos + symbol.size()) {}
  Token() : Token(TokenType::None, "", 0) {}

  TokenType type;
  std::string symbol;
  std::size_t first_char_pos;
  std::size_t last_char_pos;

  static Token Eof(std::size_t pos);
  static Token None(std::size_t pos);

  std::size_t length() const { return last_char_pos - first_char_pos; }
};

std::ostream& operator<<(std::ostream& out, Token const& t);

}  // namespace arty

#endif  // TOKEN_H
