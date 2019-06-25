#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <memory>

#include <arty/core/number.h>
#include <arty/impl/lexer.h>

namespace arty {

enum ExpressionType {
  XpNumber,
};

union ExpressionContent {
  double num;
  int symbol;
};
typedef std::shared_ptr<ExpressionContent> XpContentPtr;

struct Expression {
  ExpressionType type;
  ExpressionContent value;
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
    10,  // -
    11,  // *
    11,  // /
    1,   // number
    0,   // group
};

struct Parser {
  Lexer::Ptr _lexer;
  Token _curr_token;

  /* EXPR: calc MATH_EXPR       |
   *       cd   OPTS            |
   *       id   OPTS            |
   *       var  id = MATH_EXPR  |
   */
  Expression expression();

  /*  MATH_EXPR:    NUMBER BOP MATH_EXPR    |
   *                UOP MATH_EXPR           |
   *                NUMBER EOF              |
   */
  Expression math_expression(int rbp);

  Expression unary_operator(Token t);

  Expression binary_operator(Expression left, Token curr);

  int right_precedence(Token const& t);

  int left_precedence(Token const& t);

  void parse(Lexer::Ptr const& lexer);
};

}  // namespace arty

#endif  // PARSER_H
