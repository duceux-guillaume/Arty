#ifndef NUMBER_H
#define NUMBER_H

#include <iostream>
#include <memory>
#include <vector>

namespace arty {

class Whole {
 public:
  using digit_t = uint8_t;
  using storage_t = std::vector<digit_t>;

  Whole(unsigned long integer);
  Whole() : Whole(0) {}

  size_t length() const { return _digits.size(); }
  digit_t base() const { return _base; }

  digit_t digit(size_t i) const;
  void set_digit(size_t pos, digit_t digit);

  storage_t::const_reverse_iterator rbegin() const { return _digits.rbegin(); }
  storage_t::const_reverse_iterator rend() const { return _digits.rend(); }

  static Whole gcd(Whole const& l, Whole const& r);

  Whole& operator+=(Whole const& rhs);
  Whole& operator*=(Whole const& rhs);

 protected:
  digit_t _base;
  storage_t _digits;

 private:
  bool is_zero() const { return _digits.size() == 1 && _digits[0] == 0; }
  bool is_one() const { return _digits.size() == 1 && _digits[0] == 1; }
};

inline const Whole operator+(Whole l, Whole const& r) {
  l += r;
  return l;
}
inline const Whole operator*(Whole l, Whole const& r) {
  l *= r;
  return l;
}

Whole operator-(Whole const& l, Whole const& r);
Whole operator/(Whole l, Whole const& r);
Whole operator%(Whole l, Whole const& r);

std::ostream& operator<<(std::ostream& out, Whole const& i);

bool operator>(Whole const& l, Whole const& r);
bool operator!=(Whole const& l, Whole const& r);
bool operator<(Whole const& l, Whole const& r);
bool operator==(Whole const& l, Whole const& r);
bool operator>=(Whole const& l, Whole const& r);
bool operator<=(Whole const& l, Whole const& r);

class Integer {
 public:
  Integer() : Integer(0) {}
  Integer(Whole number) : Integer(false, std::move(number)) {}
  Integer(bool neg, Whole number) : _neg(neg), _val(std::move(number)) {}
  Integer(long integer);

  bool neg() const { return _neg; }
  Whole const& val() const { return _val; }

  friend Integer operator-(Integer const& l);
  friend Integer operator-(const Integer& l, const Integer& r);

 protected:
  bool _neg;
  Whole _val;
};

Integer operator-(Integer const& l);
Integer operator-(Integer const& l, Integer const& r);
Integer operator+(Integer const& l, Integer const& r);
Integer operator*(Integer const& l, Integer const& r);

std::ostream& operator<<(std::ostream& out, Integer const& i);

bool operator==(Integer const& l, Integer const& r);
bool operator!=(Integer const& l, Integer const& r);
bool operator>(Integer const& l, Integer const& r);
bool operator<(Integer const& l, Integer const& r);
bool operator>=(Integer const& l, Integer const& r);
bool operator<=(Integer const& l, Integer const& r);

class Rational {
 public:
  Rational(Integer const& num, Whole const& den);
  Rational() : _den(1) {}
  explicit Rational(Integer const& num) : Rational(num, Whole(1)) {}
  explicit Rational(Whole const& num) : Rational(Integer(num), Whole(1)) {}

  Integer numerator() const { return _num; }
  Whole denominator() const { return _den; }

 private:
  void simplify();

  Integer _num;
  Whole _den;
};

std::ostream& operator<<(std::ostream& out, Rational const& i);

bool operator>(Rational const& l, Rational const& r);
bool operator==(Rational const& l, Rational const& r);
bool operator!=(Rational const& l, Rational const& r);
bool operator<(Rational const& l, Rational const& r);
bool operator>=(Rational const& l, Rational const& r);
bool operator<=(Rational const& l, Rational const& r);

Rational operator+(Rational const& l, Rational const& r);
Rational operator*(Rational const& l, Rational const& r);
Rational operator/(Rational const& l, Rational const& r);
Rational operator-(Rational const& l, Rational const& r);
Rational operator-(Rational const& l);

enum NumberType {
  WHOLE,
  INTEGER,
  RATIONAL,
};

class Number {
 private:
  NumberType _type;
  std::unique_ptr<Whole> _as_whole;
  std::unique_ptr<Integer> _as_integer;
  std::unique_ptr<Rational> _as_rational;

  void upgrade_type();
  static void convert_to_greater_type(Number& l, Number& r);
  static void convert_to_minimal_type(Number& l, Number& r, NumberType type);

 public:
  Number(long integer);
  Number(long nominator, unsigned long denominator);

  Number();
  Number(Number const& other);
  Number(Number&& other);

  Number& operator=(Number const& other);
  Number& operator=(Number&& other);

  Number operator/(Number const& r) const;
  Number operator-(Number const& r) const;
  Number operator-();

  Number& operator+=(Number const& rhs);
  Number& operator*=(Number const& rhs);

  friend bool operator==(Number const& l, Number const& r);
  friend bool operator!=(Number const& l, Number const& r);
  friend bool operator>(Number const& l, Number const& r);
  friend bool operator<(Number const& l, Number const& r);
  friend bool operator>=(Number const& l, Number const& r);
  friend bool operator<=(Number const& l, Number const& r);

  friend std::ostream& operator<<(std::ostream& out, Number const& number);
};

inline const Number operator+(Number l, Number const& r) {
  l += r;
  return l;
}
inline const Number operator*(Number l, Number const& r) {
  l *= r;
  return l;
}

bool operator==(Number const& l, Number const& r);
bool operator!=(Number const& l, Number const& r);
bool operator>(Number const& l, Number const& r);
bool operator<(Number const& l, Number const& r);
bool operator>=(Number const& l, Number const& r);
bool operator<=(Number const& l, Number const& r);

std::ostream& operator<<(std::ostream& out, Number const& i);

}  // namespace arty

#endif  // Number_H
