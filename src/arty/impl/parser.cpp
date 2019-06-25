#include <arty/impl/parser.h>

namespace arty {

Expression Parser::expression() {
  switch (_curr_token.type) {
    case Calc:
      _curr_token = _lexer->next();
      return math_expression(0);
    default:
      std::cout << "Parsing error" << std::endl;
      break;
  }
  return Expression();
}

Expression Parser::math_expression(int rbp) {
  std::cout << "math_expression: " << rbp << std::endl;
  std::cout << "_curr_token: " << _curr_token << std::endl;
  Token last = _curr_token;
  _curr_token = _lexer->next();
  Expression left = unary_operator(last);
  while (rbp < right_precedence(_curr_token)) {
    last = _curr_token;
    _curr_token = _lexer->next();
    left = binary_operator(left, last);
  }
  return left;
}

Expression Parser::unary_operator(Token t) {
  std::cout << "unary_operator: " << t << std::endl;
  std::cout << "_curr_token: " << _curr_token << std::endl;
  switch (t.type) {
    case TkNumber: {
      Expression e;
      e.type = XpNumber;
      e.value.num = std::stod(t.symbol);
      return e;
    }
    case Minus: {
      Expression right = math_expression(left_precedence(t));
      right.value.num -= right.value.num;
      return right;
    }
    case TkParL: {
      Expression right = math_expression(left_precedence(t));
      // remove right par
      _curr_token = _lexer->next();  // TODO: verify token
      return right;
    }
    default: {
      return Expression();
    }
  }
}

Expression Parser::binary_operator(Expression left, Token curr) {
  std::cout << "binary_operator: " << left.value.num << " " << curr
            << std::endl;
  std::cout << "_curr_token: " << _curr_token << std::endl;
  Expression right = math_expression(right_precedence(curr));
  switch (curr.type) {
    case Plus: {
      left.value.num += right.value.num;
      return left;
    }
    case Minus: {
      left.value.num -= right.value.num;
      return left;
    }
    case Divide: {
      left.value.num /= right.value.num;
      return left;
    }
    case Times: {
      left.value.num *= right.value.num;
      return left;
    }
    default:
      break;
  }
  return left;
}

int Parser::right_precedence(const Token &t) {
  switch (t.type) {
    case Plus:
      return right_priority[OpPlus];
    case TkNumber:
      return right_priority[OpNumber];
    case Minus:
      return right_priority[OpMinus];
    case Times:
      return right_priority[OpMult];
    case Divide:
      return right_priority[OpDiv];
    default:
      return right_priority[OpGroup];
  }
}

int Parser::left_precedence(const Token &t) {
  switch (t.type) {
    case Plus:
      return left_priority[OpPlus];
    case TkNumber:
      return left_priority[OpNumber];
    case Minus:
      return left_priority[OpMinus];
    case Times:
      return left_priority[OpMult];
    case Divide:
      return left_priority[OpDiv];
    default:
      return left_priority[OpGroup];
  }
}

void Parser::parse(Lexer::Ptr const &lexer) {
  _lexer = lexer;
  _curr_token = _lexer->next();
  Expression xp = expression();
  switch (xp.type) {
    case XpNumber: {
      std::cout << xp.value.num << std::endl;
      break;
    }
  }
}

}  // namespace arty
