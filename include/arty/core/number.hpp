#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

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
  explicit operator float() const;
  explicit operator int() const;
  explicit operator std::string() const;

  // COMPARISONS
  bool operator==(number const& o) const;
  bool operator<(number const& o) const;
  bool operator>(number const& o) const;
  bool operator>=(number const& o) const;
  bool operator<=(number const& o) const;
  bool operator!=(number const& o) const;

  // OPERATORS
  number operator-() const;

  number& operator+=(number const& other);

  number& operator-=(number const& other);

  number& operator*=(number const& other);

  number& operator/=(number const& other);

  // INCR
  number& operator++();
  number operator++(int);
  number& operator--();
  number operator--(int);

  // IRR
  static number sqrt(number const& num, number const& prec);
  static number sqr(number const& n);
  static number pow(number const& b, number const& p, number const& pre);
  static number pow(number const& b, uint16_t p);

 private:
  void reduce();
  static int64_t gcd(int64_t l, int64_t r);

  bool is_integer() const;

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

constexpr double PI = 3.14159265358979323846;

static inline number sqrt(number const& n) { return number::sqrt(n, 4); }
static inline number pow(number const& n, number const& p) {
  return number::pow(n, p, 1);
}

}  // namespace arty

#endif  // NUMBER_HPP
