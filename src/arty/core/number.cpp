#include <arty/core/number.hpp>

namespace arty {

arty::number::operator std::string() const {
  if (is_integer()) {
    return std::to_string(_den);
  }
  return std::to_string(_den) + std::string("/") + std::to_string(_num);
}

number& number::operator+=(const number& other) {
  _den = _den * other._num + other._den * _num;
  _num *= other._num;
  reduce();
  return *this;
}

number& number::operator*=(const number& other) {
  _den *= other._den;
  _num *= other._num;
  reduce();
  return *this;
}

void number::reduce() {
  if (is_integer()) {
    return;
  }
  if (_num < 0) {
    _den = -_den;
    _num = -_num;
  }
  auto tmp = gcd(_num, _den);
  _num /= tmp;
  _den /= tmp;
}

int64_t number::gcd(int64_t l, int64_t r) {
  if (r < 0) {
    return gcd(l, -r);
  }
  if (l < 0) {
    return gcd(-l, r);
  }
  if (r == 0) {
    return l;
  }
  if (r == 1) {
    return r;
  }
  if (l == r) {
    return l;
  }
  if (l > r) {
    return gcd(l - r, r);
  }
  return gcd(r, r - l);
}

}  // namespace arty
