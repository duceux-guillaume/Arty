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

Whole Whole::gcd(Whole const &a, const Whole &b) {
  assert(!b.is_zero());
  Whole r(b);
  Whole old_r(a);
  Whole prov;
  while (!r.is_zero()) {
    prov = r;
    r = old_r % r;
    old_r = prov;
  }
  return old_r;
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
    _digits.clear();
    _digits.emplace_back(0);
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
  Whole tmp;
  const size_t right_size = r._digits.size();
  const size_t left_size = _digits.size();
  for (std::size_t i = 0; i < right_size; ++i) {
    tmp.set_to(0);
    uint8_t carry = 0;
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

Whole &Whole::operator/=(const Whole &rhs) {
  Whole cpy(*this);
  _digits.clear();
  _digits.push_back(0);
  while (cpy >= rhs) {
    cpy -= rhs;
    *this += 1;
  }
  return *this;
}

Whole &Whole::operator-=(const Whole &rhs) {
  if (*this <= rhs) {
    _digits.clear();
    _digits.push_back(0);
    return *this;
  }
  digit_t carry = 0;
  for (std::size_t i = 0; i < _digits.size(); ++i) {
    digit_t ld = _digits[i];
    digit_t rd = carry;
    if (i < rhs._digits.size()) {
      rd += rhs._digits[i];
    } else if (rd == 0) {
      // operation is over, we have exhausted all digit and carry to substract
      break;
    }
    if (ld < rd) {
      ld += _base;
      carry = 1;
    } else {
      carry = 0;
    }
    digit_t mod = ld - rd;
    _digits[i] = mod;
  }
  // clean up in case we are left with a bunch of trailing zeros
  while (_digits.size() > 0 && _digits.back() == 0) {
    _digits.pop_back();
  }
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

std::ostream &operator<<(std::ostream &out, const Integer &i) {
  if (i.neg()) {
    out << "-";
  }
  return out << i.val();
}

bool operator!=(const Whole &l, const Whole &r) { return !(l == r); }

bool operator<(const Whole &l, const Whole &r) { return !(l > r) && (l != r); }

bool operator==(const Whole &l, const Whole &r) {
  if (l.length() != r.length()) {
    return false;
  }
  for (auto it = l.rbegin(), it2 = r.rbegin(); it != l.rend(); ++it, ++it2) {
    if (*it != *it2) {
      return false;
    }
  }
  return true;
}

bool operator>=(const Whole &l, const Whole &r) { return (l > r) || (l == r); }

bool operator<=(const Whole &l, const Whole &r) { return (l < r) || (l == r); }

bool operator==(const Integer &l, const Integer &r) {
  return (l.neg() == r.neg()) && l.val() == r.val();
}

bool operator!=(const Integer &l, const Integer &r) { return !(l == r); }

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

Integer &Integer::operator*=(const Integer &r) {
  _neg ^= r._neg;
  _val *= r._val;
  return *this;
}

Integer &Integer::operator+=(const Integer &rhs) {
  if (!_neg && !rhs._neg) {
    _val += rhs._val;
    return *this;
  }
  if (_neg && !rhs._neg) {
    if (rhs._val >= _val) {
      _neg = false;
      _val = rhs._val - _val;
    } else {
      _val -= rhs._val;
    }
    return *this;
  }
  if (_neg && rhs._neg) {
    _val += rhs._neg;
    return *this;
  }
  // l >= 0 && r < 0 => l + (-r) == l - r
  if (_val >= rhs._val) {
    _val -= rhs._val;
  } else {
    _neg = true;
    _val = rhs._val - _val;
  }
  return *this;
}

Integer &Integer::operator-=(const Integer &r) {
  // l >= r >= 0
  if (!r._neg && *this >= r) {
    _val -= r._val;
    return *this;
  }
  // r > l >= 0
  if (!_neg && r > *this) {
    _val = r._val - _val;
    _neg = true;
    return *this;
  }
  // l < 0 <= r
  if (_neg && !r._neg) {
    _val += r._val;
    return *this;
  }
  // l >= 0 > r
  if (r._neg && !_neg) {
    _val += r._val;
    return *this;
  }
  // l < r < 0
  if (r > *this && r._neg) {
    _val -= r._val;
    return *this;
  }
  // r <= l < 0
  _neg = false;
  _val = r._val - _val;
  return *this;
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

Integer &Integer::operator-() {
  _neg = !_neg;
  return *this;
}

Rational::Rational(Integer const &num, Whole const &den)
    : _num(num), _den(den) {
  simplify();
}

Rational &Rational::operator+=(const Rational &rhs) {
  if (_den != rhs._den) {
    _num = _num * rhs._den + rhs._num * _den;
    _den *= rhs._den;
    simplify();
  } else {
    _num += rhs._num;
  }
  return *this;
}

Rational &Rational::operator*=(const Rational &rhs) {
  _num = _num * rhs._num;
  _den *= rhs._den;
  simplify();
  return *this;
}

void Rational::simplify() {
  if (is_zero()) {
    if (!_den.is_one()) {
      _den.set_to(1);
    }
    return;
  }
  if (is_integer()) {
    return;
  }
  Whole tmp = Whole::gcd(_num.val(), _den);
  if (tmp.is_one()) {
    return;
  }
  _num.val() /= tmp;
  _den /= tmp;
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

Rational &Rational::operator/=(Rational const &r) {
  assert(!r.is_zero());
  _num *= r._den;
  _den *= r._num.val();
  return *this;
}

Rational &Rational::operator-=(const Rational &rhs) {
  if (_den == rhs._den) {
    _num -= rhs._num;
  } else {
    _num = _num * rhs._den - rhs._num * _den;
    _den *= rhs._den;
    simplify();
  }
  return *this;
}

Rational &Rational::operator-() {
  _num = -_num;
  return *this;
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

Number &Number::operator-() {
  _num = -_num;
  return *this;
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

Whole &Whole::operator%=(const Whole &r) {
  while (*this >= r) {
    *this -= r;
  }
  return *this;
}

}  // namespace arty
