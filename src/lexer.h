#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>

namespace arty {

enum TokenType {
    KEYWORD
};

struct Token {
    TokenType type;
    std::string lex;
};

namespace lexer {

typedef std::unordered_map<int, std::unordered_map<char, int>> DFATransitionTable;
typedef std::unordered_set<int> DFAAcceptingSet;

class DFA {
public:
    DFAAcceptingState(DFATransitionTable table, DFAAcceptingSet accset):
        state_(0),
        accepting_(false),
        rejected_(false),
        table_(table),
        accepting_set_(accset)
    {}

    void eat(char c) {
        auto it = table_.find(state_);
        if (it == table_.end()) {
                // very bad
            rejected_ = true;
            return;
        }
        it2 = it.second.find(c);
        if (it2 == it.second.end()) {
            rejected_ = false;
            return;
        }
        state_ = it2.second;
        auto it3 = accepting_set_.find(state_);
        accepting_ = it3 != accepting_set_.end() && it3.second;
    }

    bool accepting(void) const { return accepting_; }
    bool rejected(void) const { return rejected_; }
    void reset(void) {
        state_ = 0;
        accepting_ = false;
        rejected_ = false;
    }

private:
    int state_;
    bool accepting_;
    bool rejected_;
    DFATransitionTable table_;
    DFAAcceptingSet accepting_set_;
};

struct Classifier {
    TokenType type;
    DFA dfa;
};

Classifier CreateNumberClassoifier(void) {

}

}  // lexer
}  // arty


#endif // LEXER_H
