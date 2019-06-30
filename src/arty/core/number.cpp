#include <arty/core/number.h>

#include <cassert>

namespace arty {

const Whole::digit_t DEFAULT_BASE = 10;

Whole::Whole(unsigned long integer) : _base(DEFAULT_BASE), _digits() {
  do {
    _digits.emplace_back(static_cast<uint8_t>(integer % _base));
    integer /= _base;
  } while (integer != 0);
}

Whole::digit_t Whole::digit(size_t i) const {
  if (i < length()) {
    return _digits[i];
  }
  return 0;
}

void Whole::set_digit(size_t pos, Whole::digit_t digit) {
  if (pos < _digits.size()) {
    _digits[pos] = digit;
  } else if (digit != 0) {
    do {
      _digits.emplace_back(0);
    } while (pos >= _digits.size());
    _digits[pos] = digit;
  }
}

Whole Whole::gcd(const Whole &l, const Whole &r) {
  if (r.is_zero()) {
    return l;
  }
  if (r.is_one()) {
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

Whole &Whole::operator+=(Whole const &rhs) {
  // early out
  if (is_zero()) {
    *this = rhs;
    return *this;
  }
  if (rhs.is_zero()) {
    return *this;
  }

  digit_t carry = 0;
  size_t max_length = std::max(_digits.size(), rhs._digits.size());
  for (size_t i = 0; i < max_length; ++i) {
    digit_t tmp = (i < _digits.size()) ? _digits[i] : 0;
    if (i < rhs._digits.size()) {
      tmp += rhs._digits[i] + carry;
    } else {
      tmp += carry;
    }
    digit_t mod = tmp % _base;
    carry = tmp / _base;
    if (i < _digits.size()) {
      _digits[i] = mod;
    } else {
      _digits.emplace_back(mod);
    }
  }
  if (carry > 0) {
    _digits.emplace_back(carry);
  }
  return *this;
}

Whole &Whole::operator*=(const Whole &r) {
  // early out
  if (is_zero() || r.is_zero()) {
    *this = Whole();
    return *this;
  }
  if (is_one()) {
    *this = r;
    return *this;
  }
  if (r.is_one()) {
    return *this;
  }

  Whole res;
  const size_t right_size = r._digits.size();
  for (std::size_t i = 0; i < right_size; ++i) {
    Whole tmp;
    uint8_t carry = 0;
    const size_t left_size = _digits.size();
    for (std::size_t j = 0; j < left_size; ++j) {
      Whole::digit_t dig = r._digits[i] * _digits[j] + carry;
      carry = dig / _base;
      dig = dig % _base;
      tmp.set_digit(j + i, dig);
    }
    if (carry > 0) {
      tmp.set_digit(left_size + i, carry);
    }
    res = res + tmp;
  }
  *this = std::move(res);
  return *this;
}

std::ostream &operator<<(std::ostream &out, const Whole &i) {
  if (i.length() == 0) {
    return out << '0';
  }
  for (auto it = i.rbegin(); it != i.rend(); ++it) {
    out << static_cast<int>(*it);
  }
  return out;
}

Whole operator-(const Whole &l, const Whole &r) {
  if (l < r) {
    return Whole();
  }
  Whole res;
  uint8_t carry = 0;
  for (std::size_t i = 0; i < l.length(); ++i) {
    uint8_t ld = l.digit(i);
    uint8_t rd = r.digit(i) + carry;
    if (ld < rd) {
      ld += res.base();
      carry = 1;
    }
    uint8_t mod = ld - rd;
    res.set_digit(i, mod);
  }
  return res;
}

Integer operator-(const Integer &l, const Integer &r) {
  // l >= r >= 0
  if (!r.neg() && l >= r) {
    return Integer(false, l._val - r._val);
  }
  // r >= l >= 0
  if (!l.neg() && r >= l) {
    return Integer(true, r._val - l._val);
  }
  // l < 0 <= r
  if (l.neg() && !r.neg()) {
    return Integer(true, r._val + l._val);
  }
  // l >= 0 > r
  if (r.neg() && !l.neg()) {
    return Integer(false, l._val + r._val);
  }
  // l < 0 && r < 0
  return Integer(true, r._val + l._val);
}

bool operator>(const Whole &l, const Whole &r) {
  if (l.length() > r.length()) {
    return true;
  }
  if (l.length() < r.length()) {
    return false;
  }
  for (auto it = l.rbegin(), it2 = r.rbegin(); it != l.rend(); ++it, ++it2) {
    if (*it > *it2) {
      return true;
    }
    if (*it < *it2) {
      return false;
    }
  }
  return false;
}

std::ostream &operator<<(std::ostream &out, const Integer &i) {
  if (i.neg()) {
    out << "-";
  }
  return out << i.val();
}

bool operator!=(const Whole &l, const Whole &r) { return (l > r) || (r > l); }

bool operator<(const Whole &l, const Whole &r) { return !(l > r) && (l != r); }

bool operator==(const Whole &l, const Whole &r) { return !(l != r); }

bool operator>=(const Whole &l, const Whole &r) { return (l > r) || (l == r); }

bool operator<=(const Whole &l, const Whole &r) { return (l < r) || (l == r); }

Integer operator-(const Integer &l) {
  Integer r = l;
  r._neg = !r._neg;
  return r;
}

bool operator==(const Integer &l, const Integer &r) {
  return (l.neg() == r.neg()) && l.val() == r.val();
}

bool operator!=(const Integer &l, const Integer &r) { return !(l == r); }

Integer operator+(const Integer &l, const Integer &r) {
  if (!l.neg() && !r.neg()) {
    return Integer(false, l.val() + r.val());
  }
  if (l.neg() && !r.neg()) {
    return r + l;
  }
  if (l.neg() && r.neg()) {
    return -l - r;
  }
  // l >= 0 && r < 0 => l + (-r) == l - r
  return l - (-r);
}

bool operator>(const Integer &l, const Integer &r) {
  if (l.neg() && r.neg()) {
    return r.val() > l.val();
  }
  if (l.neg() && !r.neg()) {
    return false;
  }
  if (!l.neg() && r.neg()) {
    return true;
  }
  // if (!l.neg() && !r.neg())
  return l.val() > r.val();
}

bool operator<(const Integer &l, const Integer &r) {
  return !(l > r) && (l != r);
}

bool operator>=(const Integer &l, const Integer &r) {
  return (l > r) || (l == r);
}

bool operator<=(const Integer &l, const Integer &r) {
  return (l < r) || (l == r);
}

Integer::Integer(long integer) : _neg(false), _val() {
  if (integer < 0) {
    _neg = true;
    integer = -integer;
  }
  _val = Whole(static_cast<unsigned long>(integer));
}

Integer operator*(const Integer &l, const Integer &r) {
  if (l.neg() ^ r.neg()) {
    return Integer(true, l.val() * r.val());
  }
  return Integer(false, l.val() * r.val());
}

Rational::Rational(Integer const &num, Whole const &den)
    : _num(num), _den(den) {
  simplify();
}

Rational &Rational::operator+=(const Rational &rhs) {
  _num = _num * rhs._den + rhs._num * _den;
  _den *= rhs._den;
  simplify();
  return *this;
}

Rational &Rational::operator*=(const Rational &rhs) {
  _num = _num * rhs._num;
  _den *= rhs._den;
  simplify();
  return *this;
}

void Rational::simplify() {
  if (is_integer()) {
    return;
  }
  Whole tmp = Whole::gcd(_num.val(), _den);
  _num.val() = _num.val() / tmp;
  _den = _den / tmp;
}

std::ostream &operator<<(std::ostream &out, const Rational &i) {
  return out << i.numerator() << "/" << i.denominator();
}

bool operator==(const Rational &l, const Rational &r) {
  return l.numerator() == r.numerator() && l.denominator() == r.denominator();
}

bool operator!=(const Rational &l, const Rational &r) { return !(l == r); }

bool operator>(const Rational &l, const Rational &r) {
  return l.numerator() * Integer(r.denominator()) >
         Integer(l.denominator()) * r.numerator();
}

bool operator<(const Rational &l, const Rational &r) {
  return !(l > r) && (l != r);
}

bool operator>=(const Rational &l, const Rational &r) {
  return (l > r) || (l == r);
}

bool operator<=(const Rational &l, const Rational &r) {
  return (l < r) || (l == r);
}

Rational operator/(const Rational &l, const Rational &r) {
  Rational inv(r.denominator(), r.numerator().val());
  if (r.numerator().neg()) {
    inv = inv * Rational(-1, 1);
  }
  return l * inv;
}

Rational operator-(const Rational &l, const Rational &r) {
  return Rational(l.numerator() * Integer(r.denominator()) -
                      Integer(l.denominator()) * r.numerator(),
                  l.denominator() * r.denominator());
}

Rational operator-(const Rational &l) {
  return Rational(-l.numerator(), l.denominator());
}

Number::Number(long nominator, unsigned long denominator)
    : _num(nominator, denominator) {}

Number::Number(long integer) : _num(Integer(integer)) {}

Number::Number() : _num() {}

Number::Number(const Number &other) : _num(other._num) {}

Number::Number(Number &&other) : _num(std::move(other._num)) {}

Number &Number::operator=(Number const &other) {
  _num = other._num;
  return *this;
}

Number &Number::operator=(Number &&other) {
  _num = std::move(other._num);
  return *this;
}

Number Number::operator/(const Number &other) const {
  Number result(*this);
  result._num = _num / other._num;
  return result;
}

Number Number::operator-(const Number &other) const {
  Number result(*this);
  result._num = _num - other._num;
  return result;
}

Number Number::operator-() {
  Number other(*this);
  other._num = -other._num;
  return other;
}

Number &Number::operator+=(const Number &other) {
  _num += other._num;
  return *this;
}

Number &Number::operator*=(const Number &other) {
  _num *= other._num;
  return *this;
}

std::ostream &operator<<(std::ostream &out, const Number &number) {
  return out << number._num;
}

bool operator>(const Number &l, const Number &r) { return l._num > r._num; }

bool operator!=(const Number &l, const Number &r) { return l > r || r > l; }
bool operator==(const Number &l, const Number &r) { return !(l != r); }
bool operator>=(const Number &l, const Number &r) { return l > r || l == r; }
bool operator<(const Number &l, const Number &r) { return r > l; }
bool operator<=(const Number &l, const Number &r) { return r >= l; }

Whole operator/(Whole l, const Whole &r) {
  Whole res;
  while (l >= r) {
    l = l - r;
    res = res + 1;
  }
  return res;
}

Whole operator%(Whole l, const Whole &r) {
  while (l >= r) {
    l = l - r;
  }
  return l;
}

}  // namespace arty
