#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <memory>

#include <arty/core/command.h>
#include <arty/core/number.h>
#include <arty/impl/lexer.h>

namespace arty {

enum ExpressionType {
  XpNone,
  XpNumber,
  XpCmd,
};

struct Expression {
  ~Expression();
  Expression();
  Expression(Expression const& other);
  Expression(Expression&& other);
  Expression& operator=(Expression const& other);
  Expression& operator=(Expression&&) = default;

  ExpressionType type;
  std::unique_ptr<Number> as_num;
  std::unique_ptr<Command> as_cmd;
};

// Operator Precedence, don't change the order
enum OpCode {
  OpPlus,
  OpMinus,
  OpMult,
  OpDiv,
  OpNumber,
  OpGroup,
  NumberOfOp,
};

const int right_priority[NumberOfOp] = {
    10,  // +
    10,  // -
    11,  // *
    11,  // /
    1,   // number
    0,   // group
};

const int left_priority[NumberOfOp] = {
    10,  // +
    11,  // -
    11,  // *
    11,  // /
    1,   // number
    0,   // group
};

struct Parser {
  Lexer::Ptr _lexer;
  Token _curr_token;

  /* EXPR:  calc MATH_EXPR       |
   *        CMD_EXPR             |
   *        cd   OPTS            |
   *        var  id = MATH_EXPR  |
   */
  Expression expression();

  /*  MATH_EXPR:    NUMBER BOP MATH_EXPR    |
   *                UOP MATH_EXPR           |
   *                NUMBER EOF              |
   */
  Expression math_expression(int rbp);

  /*    CMD_EXPR:   cmd EOF                 |
   *                cmd opts EOF            |
   *                cmd opts BOP CMD_EXPR   |
   */
  Expression cmd_expression();

  Expression unary_operator(Token t);

  Expression binary_operator(Expression left, Token curr);

  Number parse_number(std::string const& number);

  int right_precedence(Token const& t);

  int left_precedence(Token const& t);

  void parse(Lexer::Ptr const& lexer);
};

}  // namespace arty

#endif  // PARSER_H
