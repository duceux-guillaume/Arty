#ifndef RESULT_H
#define RESULT_H

#include <iostream>
#include <memory>

#define return_if_error(result) \
  do {                          \
    Result res = result;        \
    if (!res) {                 \
      return res;               \
    }                           \
  } while (0);

namespace arty {
template <typename T>
using Ptr = std::shared_ptr<T>;

class Result {
 public:
  Result() : _is_ok(true), _error() {}
  Result(std::string const& msg) : _is_ok(false), _error(msg) {
    std::cerr << msg << std::endl;
  }
  Result(bool b) : _is_ok(b), _error() {
    if (!_is_ok) {
      _error = std::string("unknown error");
    }
  }

  explicit operator bool() const { return _is_ok; }
  bool operator==(Result const& r) const { return _is_ok == r._is_ok; }

  std::string message() const {
    if (_is_ok) {
      return "ok";
    }
    return _error;
  }

  void panic() const {
    if (!_is_ok) {
      std::cerr << _error << std::endl;
      throw std::runtime_error(_error);
    }
  }

 private:
  bool _is_ok;
  std::string _error;
};

}  // namespace arty

std::ostream& operator<<(std::ostream& os, arty::Result const& r);

#define ok() arty::Result()
#define error(msg)                                                      \
  arty::Result(__FILE__ + std::string(":") + std::to_string(__LINE__) + \
               std::string(":") + msg)

#endif  // RESULT_H
