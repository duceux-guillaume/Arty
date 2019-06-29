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
      e.as_num.reset(new Number(parse_number(t.symbol)));
      return e;
    }
    case Minus: {
      Expression right = math_expression(left_precedence(t));
      *right.as_num = -(*right.as_num);
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
  std::cout << "binary_operator: " << *left.as_num << " " << curr << std::endl;
  std::cout << "_curr_token: " << _curr_token << std::endl;
  Expression right = math_expression(right_precedence(curr));
  switch (curr.type) {
    case Plus: {
      *left.as_num = *left.as_num + *right.as_num;
      return left;
    }
    case Minus: {
      *left.as_num = *left.as_num - *right.as_num;
      return left;
    }
    case Divide: {
      *left.as_num = *left.as_num / *right.as_num;
      return left;
    }
    case Times: {
      *left.as_num = *left.as_num * *right.as_num;
      return left;
    }
    default:
      break;
  }
  return left;
}

Number Parser::parse_number(const std::string &number) {
  return Number(std::stoi(number));
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
      std::cout << *xp.as_num << std::endl;
      break;
    }
    default:
      std::cout << "None" << std::endl;
      break;
  }
}

Expression::~Expression() {}

Expression::Expression() : type(XpNone), as_num() {}

Expression::Expression(const Expression &other) {
  type = other.type;
  switch (type) {
    case XpNone:
      break;
    case XpNumber:
      as_num.reset(new Number(*other.as_num));
      break;
  }
}

Expression::Expression(Expression &&other) {
  type = std::move(other.type);
  switch (type) {
    case XpNone:
      break;
    case XpNumber:
      as_num = std::move(other.as_num);
      break;
  }
}

}  // namespace arty
