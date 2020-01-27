#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

namespace arty {

class number {
 public:
  // CONSTRUCTORS
  number() : number(0) {}
  number(int i) : _den(i), _num(1) {}
  number(float f) : number(static_cast<double>(f)) {}
  number(double d) : number(static_cast<int64_t>(d * 1000000.), 1000000) {}
  number(int64_t dec, int64_t num) : _den(dec), _num(num) { reduce(); }

  // CAST
  explicit operator double() const;
  explicit operator int() const;
  explicit operator std::string() const;

  // COMPARISONS
  bool operator==(number const& o) const {
    return _den == o._den && _num == o._num;
  }
  bool operator<(number const& o) const {
    return _den * o._num < o._den * _num;
  }
  bool operator>(number const& o) const {
    return !(*this < o) && !(*this == o);
  }
  bool operator>=(number const& o) const { return !(*this < o); }
  bool operator<=(number const& o) const { return (*this < o) || (*this == o); }
  bool operator!=(number const& o) const { return !(*this == o); }

  // OPERATORS
  number operator-() const { return number(-_den, _num); }

  number& operator+=(number const& other);

  number& operator-=(number const& other) { return *this += (-other); }

  number& operator*=(number const& other);

  number& operator/=(number const& other) {
    return *this *= number(other._num, other._den);
  }

  // INCR
  number& operator++();
  number operator++(int);
  number& operator--();
  number operator--(int);

  // IRR
  static number sqrt(number const& num, number const& prec);
  static number sqr(number const& n);
  static number pow(number const& b, number const& p, number const& pre);

 private:
  void reduce();
  static int64_t gcd(int64_t l, int64_t r);

  bool is_integer() const { return _num == 1; }

  std::pair<int64_t, number> split() const;

  friend std::ostream& operator<<(std::ostream& os, number const& n) {
    return os << static_cast<std::string>(n);
  }

 private:
  int64_t _den;
  int64_t _num;
};

// OPERATORS
inline const number operator+(number l, number const& r) {
  l += r;
  return l;
}

inline const number operator-(number l, number const& r) {
  l -= r;
  return l;
}

inline const number operator*(number l, number const& r) {
  l *= r;
  return l;
}

inline const number operator/(number l, number const& r) {
  l /= r;
  return l;
}

}  // namespace arty

#endif  // NUMBER_HPP
