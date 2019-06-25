#include <iostream>

#include <arty/core/number.h>
#include <arty/impl/lexer.h>
#include <arty/impl/parser.h>

using namespace arty;

int main(int argc, char** argv) {
  std::string input;
  while (std::getline(std::cin, input)) {
    Lexer::Ptr lex(std::make_shared<Lexer>(input));
    Parser parser;
    parser.parse(lex);
    std::cout << *lex << std::endl;
  }

  return 0;
}
