#include <arty/core/number.h>

namespace arty {

Whole::Whole(unsigned long integer) : _digits(), _base(10) {
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
  if (r == 0) {
    return l;
  }
  if (l == r) {
    return l;
  }
  if (l > r) {
    return gcd(l - r, r);
  }
  return gcd(r, r - l);
}

Whole Whole::div(Whole l, const Whole &r) {
  Whole res;
  while (l >= r) {
    l = l - r;
    res = res + 1;
  }
  return res;
}

Whole Whole::mod(Whole l, const Whole &r) {
  while (l >= r) {
    l = l - r;
  }
  return l;
}

Whole operator*(const Whole &l, const Whole &r) {
  if (l.length() < r.length()) {
    return r * l;
  }
  Whole res;
  for (std::size_t i = 0; i < r.length(); ++i) {
    Whole tmp;
    uint8_t carry = 0;
    for (std::size_t j = 0; j < l.length(); ++j) {
      Whole::digit_t dig = r.digit(i) * l.digit(j) + carry;
      carry = dig / tmp.base();
      dig = dig % tmp.base();
      tmp.set_digit(j + i, dig);
    }
    res = res + tmp;
  }
  return res;
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

Whole operator+(const Whole &l, const Whole &r) {
  if (l.length() < r.length()) {
    return r + l;
  }
  Whole res;
  uint8_t carry = 0;
  for (std::size_t i = 0; i < l.length(); ++i) {
    uint8_t tmp = l.digit(i);
    if (i < r.length()) {
      tmp += r.digit(i) + carry;
    } else {
      tmp = tmp + carry;
    }
    uint8_t mod = static_cast<uint8_t>(tmp % res.base());
    carry = tmp / res.base();
    res.set_digit(i, mod);
  }
  if (carry > 0) {
    res.set_digit(res.length(), carry);
  }
  return res;
}

Integer operator-(const Whole &l, const Whole &r) {
  if (l < r) {
    return -(r - l);
  }
  Integer res;
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
    return Whole(l) - Whole(r);
  }
  // r >= l >= 0
  if (!l.neg() && r >= l) {
    return -(Whole(r) - Whole(l));
  }
  // l < 0 <= r
  if (l.neg() && !r.neg()) {
    return -(Whole(l) + Whole(r));
  }
  // l >= 0 > r
  if (r.neg() && !l.neg()) {
    return Whole(l) + Whole(r);
  }
  // l < 0 && r < 0
  return -Integer(Whole(l) + Whole(r));
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
  return out << Whole(i);
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
  return (l.neg() == r.neg()) && (Whole(l) == Whole(r));
}

bool operator!=(const Integer &l, const Integer &r) { return !(l == r); }

Integer operator+(const Integer &l, const Integer &r) {
  if (!l.neg() && !r.neg()) {
    return Whole(l) + Whole(r);
  }
  if (l.neg() && !r.neg()) {
    return r + l;
  }
  if (l.neg() && r.neg()) {
    return -(l + r);
  }
  // l >= 0 && r < 0 => l + (-r) == l - r
  return l - (-r);
}

bool operator>(const Integer &l, const Integer &r) {
  if (l.neg() && r.neg()) {
    return Whole(r) > Whole(l);
  }
  if (l.neg() && !r.neg()) {
    return false;
  }
  if (!l.neg() && r.neg()) {
    return true;
  }
  // if (!l.neg() && !r.neg())
  return Whole(l) > Whole(r);
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

Integer::Integer(long integer) : Whole(), _neg(false) {
  if (integer < 0) {
    _neg = true;
    integer = -integer;
  }
  _digits.clear();
  do {
    _digits.emplace_back(static_cast<uint8_t>(integer % _base));
    integer /= _base;
  } while (integer != 0);
}

Integer operator*(const Integer &l, const Integer &r) {
  if (l.neg() ^ r.neg()) {
    return -Integer(Whole(l) * Whole(r));
  }
  return Whole(l) * Whole(r);
}

Rational::Rational(Integer const &num, Whole const &den)
    : _num(num), _den(den) {
  simplify();
}

void Rational::simplify() {
  bool neg = _num.neg();
  Whole tmp = Whole::gcd(_num, _den);
  _num = Whole::div(_num, tmp);
  if (neg) {
    _num = -_num;
  }
  _den = Whole::div(_den, tmp);
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

Rational operator+(const Rational &l, const Rational &r) {
  return Rational(l.numerator() * Integer(r.denominator()) +
                      Integer(l.denominator()) * r.numerator(),
                  l.denominator() * r.denominator());
}

Rational operator*(const Rational &l, const Rational &r) {
  return Rational(l.numerator() * r.numerator(),
                  l.denominator() * r.denominator());
}

Rational operator/(const Rational &l, const Rational &r) {
  Rational inv(r.denominator(), r.numerator());
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

void Number::upgrade_type() {
  switch (_type) {
    case WHOLE:
      _type = INTEGER;
      _as_integer.reset(new Integer(*_as_whole));
      break;
    case INTEGER:
      _type = RATIONAL;
      _as_rational.reset(new Rational(*_as_integer));
      break;
    case RATIONAL:
      break;
  }
}

void Number::convert_to_greater_type(Number &l, Number &r) {
  while (l._type < r._type) {
    l.upgrade_type();
  }
  while (r._type < l._type) {
    r.upgrade_type();
  }
}

void Number::convert_to_minimal_type(Number &l, Number &r, NumberType type) {
  while (l._type < type) {
    l.upgrade_type();
  }
  while (r._type < type) {
    r.upgrade_type();
  }
  convert_to_greater_type(l, r);
}

Number::Number(long nominator, unsigned long denominator) {
  _type = RATIONAL;
  _as_rational.reset(new Rational(nominator, denominator));
}

Number::Number(long integer) {
  if (integer >= 0) {
    _type = WHOLE;
    _as_whole.reset(new Whole(static_cast<unsigned long>(integer)));
  } else {
    _type = INTEGER;
    _as_integer.reset(new Integer(static_cast<long>(integer)));
  }
}

Number::Number()
    : _type(WHOLE),
      _as_whole(std::make_unique<Whole>()),
      _as_integer(),
      _as_rational() {}

Number::Number(const Number &other) {
  _type = other._type;
  switch (_type) {
    case WHOLE:
      _as_whole.reset(new Whole(*other._as_whole));
      break;
    case INTEGER:
      _as_integer.reset(new Integer(*other._as_integer));
      break;
    case RATIONAL:
      _as_rational.reset(new Rational(*other._as_rational));
      break;
  }
}

Number &Number::operator=(Number const &other) {
  if (this != &other) {
    switch (other._type) {
      case WHOLE:
        _as_whole.reset(new Whole(*other._as_whole));
        break;
      case INTEGER:
        _as_integer.reset(new Integer(*other._as_integer));
        break;
      case RATIONAL:
        _as_rational.reset(new Rational(*other._as_rational));
        break;
    }
    _type = other._type;
  }
  return *this;
}

Number Number::operator+(const Number &other) {
  if (_type != other._type) {
    Number real_other(other);
    convert_to_greater_type(*this, real_other);
    return *this + real_other;
  }
  Number result(*this);
  switch (_type) {
    case WHOLE:
      *result._as_whole = *_as_whole + *other._as_whole;
      break;
    case INTEGER:
      *result._as_integer = *_as_integer + *other._as_integer;
      break;
    case RATIONAL:
      *result._as_rational = *_as_rational + *other._as_rational;
      break;
  }
  return result;
}

Number Number::operator*(const Number &other) {
  if (_type != other._type) {
    Number real_other(other);
    convert_to_greater_type(*this, real_other);
    return *this * real_other;
  }
  Number result(*this);
  switch (_type) {
    case WHOLE:
      *result._as_whole = *_as_whole * *other._as_whole;
      break;
    case INTEGER:
      *result._as_integer = *_as_integer * *other._as_integer;
      break;
    case RATIONAL:
      *result._as_rational = *_as_rational * *other._as_rational;
      break;
  }
  return result;
}

Number Number::operator/(const Number &other) {
  if (_type != other._type || _type < RATIONAL) {
    Number real_other(other);
    convert_to_minimal_type(*this, real_other, RATIONAL);
    return *this / real_other;
  }
  Number result(*this);
  *result._as_rational = *_as_rational / *other._as_rational;
  return result;
}

Number Number::operator-(const Number &other) {
  if (_type != other._type || _type < INTEGER) {
    Number real_other(other);
    convert_to_minimal_type(*this, real_other, INTEGER);
    return *this - real_other;
  }
  Number result(*this);
  switch (_type) {
    default:
    case INTEGER:
      *result._as_integer = *_as_integer - *other._as_integer;
      break;
    case RATIONAL:
      *result._as_rational = *_as_rational - *other._as_rational;
      break;
  }
  return result;
}

Number Number::operator-() {
  Number other(*this);
  if (other._type < INTEGER) {
    other.upgrade_type();
  }
  switch (other._type) {
    default:
    case INTEGER:
      *other._as_integer = -*other._as_integer;
      break;
    case RATIONAL:
      *other._as_rational = -*other._as_rational;
      break;
  }
  return other;
}

std::ostream &operator<<(std::ostream &out, const Number &number) {
  switch (number._type) {
    case WHOLE:
      return out << *number._as_whole;
    case INTEGER:
      return out << *number._as_integer;
    case RATIONAL:
      return out << *number._as_rational;
  }
}

bool operator>(const Number &l, const Number &r) {
  if (l._type != r._type) {
    Number real_r(r);
    Number real_l(l);
    Number::convert_to_greater_type(real_l, real_r);
    return real_l > real_r;
  }
  switch (l._type) {
    case WHOLE:
      return *l._as_whole > *r._as_whole;
    case INTEGER:
      return *l._as_integer > *r._as_integer;
    case RATIONAL:
      return *l._as_rational > *r._as_rational;
  }
}

bool operator!=(const Number &l, const Number &r) { return l > r || r > l; }
bool operator==(const Number &l, const Number &r) { return !(l != r); }
bool operator>=(const Number &l, const Number &r) { return l > r || l == r; }
bool operator<(const Number &l, const Number &r) { return r > l; }
bool operator<=(const Number &l, const Number &r) { return r >= l; }

}  // namespace arty
