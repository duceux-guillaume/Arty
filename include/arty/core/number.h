#ifndef NUMBER_H
#define NUMBER_H

#include <iostream>
#include <memory>
#include <vector>

namespace arty {

class Whole {
 public:
  typedef uint8_t digit_t;
  typedef std::vector<digit_t> storage_t;

  Whole(unsigned long integer);
  Whole() : Whole(0) {}

  size_t length() const { return _digits.size(); }
  digit_t base() const { return _base; }

  digit_t digit(size_t i) const;
  void set_digit(size_t pos, digit_t digit);

  storage_t::const_reverse_iterator rbegin() const { return _digits.rbegin(); }
  storage_t::const_reverse_iterator rend() const { return _digits.rend(); }

  static Whole gcd(Whole const& l, Whole const& r);
  static Whole div(Whole l, Whole const& r);
  static Whole mod(Whole l, Whole const& r);

 protected:
  storage_t _digits;
  digit_t _base;
};

Whole operator+(Whole const& l, Whole const& r);
Whole operator*(Whole const& l, Whole const& r);

std::ostream& operator<<(std::ostream& out, Whole const& i);

bool operator>(Whole const& l, Whole const& r);
bool operator!=(Whole const& l, Whole const& r);
bool operator<(Whole const& l, Whole const& r);
bool operator==(Whole const& l, Whole const& r);
bool operator>=(Whole const& l, Whole const& r);
bool operator<=(Whole const& l, Whole const& r);

class Integer : public Whole {
 public:
  Integer() : Integer(0) {}
  Integer(Whole number) : Whole(number), _neg(false) {}
  Integer(long integer);

  bool neg() const { return _neg; }

  friend Integer operator-(Integer const& l);

 protected:
  bool _neg;
};

Integer operator-(Integer const& l);
Integer operator-(Whole const& l, Whole const& r);
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
  Rational() : _num(), _den(1) {}
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

  Number& operator=(Number const& other);

  Number operator+(Number const& r);
  Number operator*(Number const& r);
  Number operator/(Number const& r);
  Number operator-(Number const& r);
  Number operator-();

  friend bool operator==(Number const& l, Number const& r);
  friend bool operator!=(Number const& l, Number const& r);
  friend bool operator>(Number const& l, Number const& r);
  friend bool operator<(Number const& l, Number const& r);
  friend bool operator>=(Number const& l, Number const& r);
  friend bool operator<=(Number const& l, Number const& r);

  friend std::ostream& operator<<(std::ostream& out, Number const& i);
};

bool operator==(Number const& l, Number const& r);
bool operator!=(Number const& l, Number const& r);
bool operator>(Number const& l, Number const& r);
bool operator<(Number const& l, Number const& r);
bool operator>=(Number const& l, Number const& r);
bool operator<=(Number const& l, Number const& r);

std::ostream& operator<<(std::ostream& out, Number const& i);

}  // namespace arty

#endif  // Number_H
