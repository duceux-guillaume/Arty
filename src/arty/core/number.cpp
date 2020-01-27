#include <arty/core/number.hpp>

namespace arty {

arty::number::operator std::string() const {
  if (is_integer()) {
    return std::to_string(_den);
  }
  return std::to_string(_den) + std::string("/") + std::to_string(_num);
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

  // for computing integral part
  // of square root of number
  while (start <= end) {
    mid = (start + end) / 2;
    if (number(mid * mid) == num) {
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
  float increment = 0.1;
  for (number i = 0; i < prec; ++i) {
    while (ans * ans <= num) {
      ans += increment;
    }

    // loop terminates when ans * ans > number
    ans = ans - increment;
    increment = increment / 10;
  }
  return ans;
}

number number::sqr(const number& n) { return n * n; }

number number::pow(const number& b, const number& p, const number& pre) {
  if (p < 0) {
    return number(1) / number::pow(b, -p, pre);
  }
  if (p.is_integer() && p >= 0) {
    number res(1);
    for (number i(0); i < p; ++i) {
      res *= b;
    }
    return res;
  }
  if (p >= 10) {
    return number::sqr(number::pow(b, p / 2, pre / 2));
  }
  if (p >= 1) return b * number::pow(b, p - 1, pre);
  if (pre >= 1) return number::sqrt(b, pre);
  return number::sqrt(number::pow(b, p * 2, pre * 2), pre);
  return number();
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

std::pair<int64_t, number> number::split() const {
  int64_t res = 0;
  int64_t den = _den;
  while (den >= _num) {
    den -= _num;
    ++res;
  }
  return std::make_pair(res, number(den, _num));
}

}  // namespace arty
