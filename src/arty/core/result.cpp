#include <arty/core/result.h>

std::ostream &operator<<(std::ostream &os, const arty::Result &r) {
  os << r.message();
  return os;
}
