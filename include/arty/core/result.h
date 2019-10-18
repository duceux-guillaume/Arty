#ifndef RESULT_H
#define RESULT_H

#include <iostream>
#include <memory>

#define check_result(result) \
  do {                       \
    if (!result) {           \
      return result;         \
    }                        \
  } while (0)

namespace arty {
template <typename T>
using Ptr = std::shared_ptr<T>;

class Result {
 public:
  Result() : _is_ok(true), _error() {}
  Result(std::string const &msg) : _is_ok(false), _error(msg) {}

  operator bool() const { return _is_ok; }
  bool operator==(Result const &r) const { return _is_ok == r._is_ok; }

  std::string message() const {
    if (_is_ok) {
      return "ok";
    }
    return _error;
  }

 private:
  bool _is_ok;
  std::string _error;
};

}  // namespace arty

std::ostream &operator<<(std::ostream &os, arty::Result const &r);

#define ok() arty::Result()
#define error(msg) arty::Result(msg)

#endif  // RESULT_H
