#include <arty/core/number.hpp>

namespace arty {

arty::number::operator std::string() const {
  if (is_integer()) {
    return std::to_string(_den);
  }
  int64_t den = _den;
  std::string res;
  if (den < 0) {
    den = -den;
    res += "-";
  }
  int64_t div = den / _num;
  int64_t mod = den % _num;
  res += std::to_string(div) + ".";
  for (std::size_t i = 0; i < 5; ++i) {
    mod *= 10;
    div = mod / _num;
    mod = mod % _num;
    res += std::to_string(div);
    if (mod == 0) {
      break;
    }
  }
  if (mod != 0) {
    return std::to_string(_den) + "/" + std::to_string(_num);
  }
  return res;
}

arty::number::operator int() const {
  if (is_integer()) {
    return _den;
  }
  return split().first;
}

arty::number::operator double() const {
  auto s = split();
  return s.first + static_cast<double>(s.second._den) / s.second._num;
}

arty::number::operator float() const {
  return static_cast<float>(static_cast<double>(*this));
}

bool number::operator==(const number& o) const {
  return _den == o._den && _num == o._num;
}

bool number::operator<(const number& o) const {
  return _den * o._num < o._den * _num;
}

bool number::operator>(const number& o) const {
  return !(*this < o) && !(*this == o);
}

bool number::operator>=(const number& o) const { return !(*this < o); }

bool number::operator<=(const number& o) const {
  return (*this < o) || (*this == o);
}

bool number::operator!=(const number& o) const { return !(*this == o); }

number number::operator-() const { return number(-_den, _num); }

number& number::operator+=(const number& other) {
  if (_num == other._num) {
    _den += other._den;
  } else {
    auto m = gcd(_num, other._num);
    _den = _den * (other._num / m) + other._den * (_num / m);
    _num *= (other._num / m);
  }
  reduce();
  return *this;
}

number& number::operator-=(const number& other) { return *this += (-other); }

number& number::operator*=(const number& other) {
  auto r1 = gcd(_den, other._num);
  _den /= r1;
  auto onum = other._num / r1;
  auto r2 = gcd(other._den, _num);
  _num /= r2;
  auto oden = other._den / r2;
  _den *= oden;
  _num *= onum;
  reduce();
  return *this;
}

number& number::operator/=(const number& other) {
  assert(other._den > 0);
  return *this *= number(other._num, other._den);
}

number& number::operator++() {
  ++_den;
  return *this;
}

number number::operator++(int) {
  number tmp(*this);
  ++(*this);
  return tmp;
}

number& number::operator--() {
  --_den;
  return *this;
}

number number::operator--(int) {
  number tmp(*this);
  --(*this);
  return tmp;
}

number number::sqrt(const number& num, const number& prec) {
  assert(num >= 0);
  int start = 0, end = static_cast<int>(num);
  int mid;

  // variable to store the answer
  number ans;
  bool is_int = num.is_integer();

  // for computing integral part
  // of square root of number
  while (start <= end) {
    // int >> 1 is a division by 2
    mid = (start + end) >> 1;
    if (is_int && num._den == mid * mid) {
      ans = mid;
      break;
    }

    // incrementing start if integral
    // part lies on right side of the mid
    if (number(mid * mid) < num) {
      start = mid + 1;
      ans = mid;
    }

    // decrementing end if integral part
    // lies on the left side of the mid
    else {
      end = mid - 1;
    }
  }

  // For computing the fractional part
  // of square root upto given precision
  number increment(1, 10);
  double precision = static_cast<double>(prec);
  for (double i = 0; i < precision; ++i) {
    // ans*ans <= num without reducing
    while ((ans._den * ans._den * num._num) <=
           (num._den * ans._num * ans._num)) {
      ans += increment;
    }

    // loop terminates when ans * ans > number
    ans -= increment;
    increment._num *= 10;
  }
  return ans;
}

number number::sqr(const number& n) { return n * n; }

number number::pow(const number& b, const number& p, const number& pre) {
  if (p < 0) {
    return number(1) / number::pow(b, -p, pre);
  }
  if (p.is_integer() && p >= 0) {
    return number::pow(b, p._den);
  }
  if (p >= 10) {
    return number::sqr(number::pow(b, p / 2, pre / 2));
  }
  if (p >= 1) return b * number::pow(b, p - 1, pre);
  if (pre >= 1) return number::sqrt(b, pre);
  return number::sqrt(number::pow(b, p * 2, pre * 2), pre);
}

number number::pow(const number& b, uint16_t p) {
  number res(1);
  for (uint16_t i = 0; i < p; ++i) {
    res *= b;
  }
  return res;
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
  if (tmp != 0) {
    _num /= tmp;
    _den /= tmp;
  }
}

int64_t number::gcd(int64_t l, int64_t r) {
  r = (r < 0) ? -r : r;
  l = (l < 0) ? -l : l;
  auto t = r;
  while (r != 0) {
    t = r;
    r = l % r;
    l = t;
  }
  return l;
}

bool number::is_integer() const { return _num == 1; }

std::pair<int64_t, number> number::split() const {
  return std::make_pair(_den / _num, number(_den % _num, _num));
}

}  // namespace arty
