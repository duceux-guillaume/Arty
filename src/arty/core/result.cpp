#include <arty/core/result.hpp>

std::ostream &operator<<(std::ostream &os, const arty::Result &r) {
  os << r.message();
  return os;
}
