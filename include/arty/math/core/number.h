#ifndef NUMBER_H
#define NUMBER_H

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

protected:
    storage_t _digits;
    digit_t _base;
};

Whole operator+(Whole const& l, Whole const& r);
Whole operator*(Whole const& l, Whole const& r);

std::ostream& operator<<(std::ostream& out, arty::math::Whole const& i);

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

}
}  // namespace arty

#endif // INTEGER_H
