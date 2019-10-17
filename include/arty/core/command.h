#ifndef COMMAND_H
#define COMMAND_H

#include <string>

namespace arty {

class Command {
 public:
  Command& bin(std::string const& bin) {
    _bin = bin;
    return *this;
  }

  int execute() { return std::system(_bin.c_str()); }

 private:
  std::string _bin;
};

}  // namespace arty

#endif  // COMMAND_H
