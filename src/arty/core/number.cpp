#include <arty/core/number.hpp>

namespace arty {

arty::number::operator std::string() const {
  if (_num == 0) {
    return "und";
  }
  if (_den == 0 || _num == 1) {
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
  if (isInt()) {
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
  if (_den == 0) {
    return o._den == 0;
  }
  if (_num == 0) {
    return o._num == 0;
  }
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
  // Integers
  if (isInf() && other.isInf()) {
    if ((*this > 0) && (other < 0)) {
      // INF - INF
      _num = 0;
      return *this;
    }
    if ((*this < 0) && (other > 0)) {
      // -INF + INF
      _num = 0;
      return *this;
    }
    return *this;
  }

  if (isInt() && other.isInt()) {
    if (details::addIsSafe(_den, other._den)) {
      _den += other._den;
    } else {
      _den = details::INTMAX;
    }
    return *this;
  }
  // Same num
  if (_num == other._num) {
    if (details::addIsSafe(_den, other._den)) {
      _den += other._den;
    } else {
      reducePrecision();
      auto cpy = other;
      cpy.reducePrecision();
      return *this += cpy;
    }
    reduce();
    return *this;
  }

  auto numgcd = details::gcd(_num, other._num);
  auto leftnum = other._num / numgcd;
  auto rightnum = _num / numgcd;
  if (!details::multIsSafe(_num, leftnum)) {
    reducePrecision();
    auto cpy = other;
    cpy.reducePrecision();
    return *this += cpy;
  }
  _num *= (other._num / numgcd);

  if (details::multIsSafe(_den, leftnum) &&
      details::multIsSafe(other._den, rightnum)) {
    auto leftden = _den * leftnum;
    auto rightden = other._den * rightnum;
    if (details::addIsSafe(leftden, rightden)) {
      _den = leftden + rightden;
    } else {
    }
  } else {
    reducePrecision();
    auto cpy = other;
    cpy.reducePrecision();
    return *this += cpy;
  }
  return *this;
}

number& number::operator-=(const number& other) { return *this += (-other); }

number& number::operator*=(const number& other) {
  auto r1 = details::gcd(_den, other._num);
  _den /= r1;
  auto onum = other._num / r1;
  auto r2 = details::gcd(other._den, _num);
  _num /= r2;
  auto oden = other._den / r2;
  if (details::multIsSafe(_den, oden) && details::multIsSafe(_num, onum)) {
    _den *= oden;
    _num *= onum;
    reduce();
  } else {
    reducePrecision();
    auto cpy = other;
    cpy.reducePrecision();
    return *this *= cpy;
  }
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
  bool is_int = num.isInt();

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
  if (p.isInt() && p >= 0) {
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
  if (_den == 0) {
    _num = 1;
    return;
  }
  if (_num == 1) {
    return;
  }
  if (_num < 0) {
    _den = -_den;
    _num = -_num;
  }
  if (_den == _num) {
    _den = 1;
    _num = 1;
    return;
  }
  if (-_den == _num) {
    _den = -1;
    _num = 1;
    return;
  }
  auto tmp = details::gcd(_num, _den);
  if (tmp != 0) {
    _num /= tmp;
    _den /= tmp;
  }
}

void number::reducePrecision() {
  if (_den == 0 || _num == 1) {
    return;
  }
  _den = _den >> 1;
  _num = (_num & 1) ? _num >> 1 : (_num >> 1) + 1;
  assert(_num > 0);
  reduce();
}

int64_t details::gcd(int64_t l, int64_t r) {
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

bool number::isInt() const { return _num == 1; }

bool number::isInf() const {
  auto max = _den == details::INTMAX;
  auto min = _den == details::INTMIN;
  auto min_ish = (_den == details::INTMIN + 1);
  return max || min || min_ish;
}

bool number::isDec() const { return !isInt(); }

number number::inf() { return number(details::INTMAX, 1); }

number number::max() { return number(details::INTMAX - 1, 1); }

number number::min() { return number(details::INTMIN + 2, 1); }

number number::eps() { return number(1, details::INTMAX); }

number number::und() { return number(1, 0); }

std::pair<int64_t, number> number::split() const {
  return std::make_pair(_den / _num, number(_den % _num, _num));
}

bool details::addIsSafe(int64_t a, int64_t b) {
  if ((b > 0) && (a > INTMAX - b)) return false;
  if ((b < 0) && (a < INTMIN - b)) return false;
  return true;
}

bool details::subIsSafe(int64_t a, int64_t b) {
  if ((b < 0) && (a > INTMAX + b)) return false;
  if ((b > 0) && (a < INTMIN + b)) return false;
  return true;
}

bool details::multIsSafe(int64_t a, int64_t b) {
  if (b == 0 || a == 0) {
    return true;
  }
  if (b == 1 || a == 1) {
    return true;
  }
  // There may be a need to check for -1 for two's complement machines.
  // If one number is -1 and another is INT_MIN, multiplying them we get
  // abs(INT_MIN) which is 1 higher than INT_MAX
  if ((a == -1) && (b == INTMIN)) {
    return false;
  }
  if ((b == -1) && (a == INTMIN)) {
    return false;
  }
  // general case
  if (b > 0) {
    if (a > INTMAX / b) {
      return false;
    }
    if ((a < INTMIN / b)) {
      return false;
    }
  } else {
    if (a > INTMAX / (-b)) {
      return false;
    }
    if ((a < INTMIN / (-b))) {
      return false;
    }
  }
  return true;
}

}  // namespace arty
