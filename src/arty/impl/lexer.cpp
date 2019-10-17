#include <arty/impl/lexer.h>

namespace arty {

Lexer::Lexer(std::string &input)
    : _input(input), _pos(0), _last_token_pos(0), _automatas(), _tokens() {
  if (_input.size() == 0 || _input.back() != '\n') {
    _input.push_back('\n');
  }
}

Token Lexer::next() {
  Token t;
  do {
    process();
    t = _tokens.back();
  } while (t.type == None);
  return t;
}

void Lexer::process() {
  reset();
  while (_pos < _input.size()) {
    char c = _input[_pos];
    Token acc_token = Token::None(_pos);
    size_t ong_count = 0;
    for (auto &l : _automatas) {
      switch (l->eat(c, _pos)) {
        case Acc:
          acc_token = l->token();
          break;
        case Ong:
          ong_count += 1;
          break;
        default:
          break;
      }
    }
    // If we got an accepted on this char, we need to parse it again after
    // global reset
    if (ong_count == 0) {
      reset();
      switch (acc_token.type) {
        default:
          _tokens.push_back(acc_token);
          _pos = _last_token_pos + acc_token.symbol.size();
          _last_token_pos = _pos;
          return;
        case None:
          _tokens.push_back(acc_token);
          _pos += 1;
          _last_token_pos = _pos;
          return;
      }
    } else {
      _pos += 1;
    }
  }
  if (_tokens.size() == 0 || _tokens.back().type != Eof) {
    _tokens.push_back(Token::Eof(_pos));
  }
}

void Lexer::reset() {
  _automatas.clear();
  // !! Error needs to be first !!
  _automatas.emplace_back(std::make_unique<ErrorFsm>());
  _automatas.emplace_back(std::make_unique<NumberFsm>());
  _automatas.emplace_back(std::make_unique<SingleCharFsm>('+', Plus));
  _automatas.emplace_back(std::make_unique<SingleCharFsm>('-', Minus));
  _automatas.emplace_back(std::make_unique<SingleCharFsm>('*', Times));
  _automatas.emplace_back(std::make_unique<SingleCharFsm>('/', Divide));
  _automatas.emplace_back(std::make_unique<SingleCharFsm>('(', TkParL));
  _automatas.emplace_back(std::make_unique<SingleCharFsm>(')', TkParR));
  _automatas.emplace_back(std::make_unique<CmdFsm>());
  _automatas.emplace_back(std::make_unique<WordFsm>("calc", Calc));
  _automatas.emplace_back(std::make_unique<WordFsm>("cd", TkCd));
  _automatas.emplace_back(std::make_unique<WordFsm>("if", TkIf));
  _automatas.emplace_back(std::make_unique<WordFsm>("for", TkFor));
  _automatas.emplace_back(std::make_unique<WordFsm>("var", TkVar));
  _automatas.emplace_back(std::make_unique<WordFsm>("while", TkWhile));
}

BaseFsm::BaseFsm() : _state(Ong), _pos(0) {}

BaseFsm::~BaseFsm() {}

FsmState BaseFsm::eat(char /*c*/, std::size_t pos) {
  _pos = pos;
  return _state;
}

Token BaseFsm::token() { return Token::Eof(_pos); }

FsmState SingleCharFsm::eat(char c, std::size_t pos) {
  BaseFsm::eat(c, pos);
  if (_state == Ong && c == _char) {
    _state = Acc;
  } else {
    _state = Rej;
  }
  return _state;
}

Token SingleCharFsm::token() {
  return Token(_type, std::string(1, _char), _pos - 1);
}

FsmState WordFsm::eat(char c, std::size_t pos) {
  BaseFsm::eat(c, pos);
  if (_state == Acc) {
    return _state;
  }
  if (_state == Ong && _word[_index] == c) {
    ++_index;
    if (_index >= _word.size()) {
      _state = Acc;
    }
    return _state;
  }
  _state = Rej;
  return _state;
}

Token WordFsm::token() { return Token(_type, _word, _pos - _word.length()); }

FsmState ErrorFsm::eat(char c, std::size_t pos) {
  BaseFsm::eat(c, pos);
  if (_word.size() == 0 && (c == ' ' || c == '\n')) {
    _state = Rej;
  } else {
    _word.push_back(c);
    _state = Acc;
  }
  return _state;
}

Token ErrorFsm::token() { return Token(Error, _word, _pos - _word.size()); }

std::ostream &operator<<(std::ostream &out, const Lexer &l) {
  out << l._input << std::endl;
  for (Token const &t : l._tokens) {
    if (t.type != Error) {
      out << "^";
      if (t.length() > 1) {
        for (std::size_t i = 0; i < t.length() - 1; ++i) {
          out << ".";
        }
      }
    } else {
      out << "|";
      if (t.length() > 1) {
        for (std::size_t i = 0; i < t.length() - 1; ++i) {
          out << "-";
        }
      }
      out << "| syntaxic error at: " << t.last_char_pos << " (" << t.symbol
          << ")";
      break;
    }
  }
  out << std::endl;
  for (Token const &t : l._tokens) {
    if (t.type != None) {
      out << t;
    }
  }
  out << std::endl;
  return out;
}

FsmState NumberFsm::eat(char c, std::size_t pos) {
  BaseFsm::eat(c, pos);
  if (_state == Ong) {
    bool isdigit = std::isdigit(static_cast<unsigned char>(c)) != 0;
    if (isdigit) {
      _word.push_back(c);
    } else if (_word.size() > 0 && !isdigit) {
      _state = Acc;
    } else {
      _state = Rej;
    }
  }
  return _state;
}

Token NumberFsm::token() { return Token(TkNumber, _word, _pos - _word.size()); }

FsmState CmdFsm::eat(char c, std::size_t pos) {
  BaseFsm::eat(c, pos);
  // Handle first char
  if (_state == Ong && _cmd.size() == 0) {
    bool isalpha = std::isalpha(static_cast<unsigned char>(c)) != 0;
    if (isalpha) {
      _cmd.push_back(c);
    } else {
      _state = Rej;
    }
    return _state;
  }

  if (_state == Ong) {
    bool isalphanum = std::isalnum(static_cast<unsigned char>(c)) != 0;
    if (isalphanum) {
      _cmd.push_back(c);
    } else {
      _state = Acc;
    }
  }
  return _state;
}

Token CmdFsm::token() { return Token(TkCmd, _cmd, _pos - _cmd.size()); }

}  // namespace arty
