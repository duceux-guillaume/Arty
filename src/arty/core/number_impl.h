#ifndef NUMBER_IMPL_H
#define NUMBER_IMPL_H

#include <arty/math/core/number.h>
#include <stdint.h>
#include <vector>
#include <utility>
#include <iostream>

namespace arty {
namespace math {

class Whole {
public:
    typedef uint8_t digit_t;
    typedef std::vector<digit_t> storage_t;

    Whole(unsigned long integer);
    Whole(): Whole(0) {}

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

class Integer: public Whole {
public:
    Integer(): Integer(0) {}
    Integer(Whole number): Whole(number), _neg(false) {}
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
    Rational(): _num(), _den(1) {}
    explicit Rational(Integer const& num): Rational(num, Whole(1)) {}
    explicit Rational(Whole const& num): Rational(Integer(num), Whole(1)) {}

    Integer numerator() const { return _num; }
    Whole denominator() const { return  _den; }
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
Rational operator-(Rational  const& l);

struct Number::NumberImpl {
    NumberImpl(Rational const& num): _num(num) {}
    NumberImpl() = default;
    NumberImpl(NumberImpl const& other) = default;
    NumberImpl(long i): _num(Integer(i)) {}
    NumberImpl(long i, unsigned long j): _num(Integer(i), Whole(j)) {}
    ~NumberImpl();

    Rational _num;

    bool operator>(NumberImpl const& r) {
        return _num > r._num;
    }
    bool operator==(NumberImpl const& r) {
        return _num == r._num;
    }
    bool operator!=(NumberImpl const& r) {
        return _num != r._num;
    }
    bool operator<(NumberImpl const& r) {
        return _num < r._num;
    }
    bool operator>=(NumberImpl const& r) {
        return _num >= r._num;
    }
    bool operator<=(NumberImpl const& r) {
        return _num <= r._num;
    }

    NumberImpl operator+(NumberImpl const& r) {
        return _num + r._num;
    }
    NumberImpl operator*(NumberImpl const& r) {
        return _num * r._num;
    }
    NumberImpl operator/(NumberImpl const& r) {
        return _num / r._num;
    }
    NumberImpl operator-(NumberImpl const& r) {
        return _num - r._num;
    }
    NumberImpl operator-() {
        return -_num;
    }
};

}  // math
}  // arty

#endif // NUMBER_IMPL_H
