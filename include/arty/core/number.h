#ifndef NUMBER_H
#define NUMBER_H

#include <iostream>
#include <memory>

namespace arty {

class Number {
private:
    struct NumberImpl;
    std::unique_ptr< NumberImpl > _impl;
    Number(NumberImpl const& impl);

public:
    Number();
    Number(Number const& other);
    Number(Number&&) = default;
    Number& operator=(Number const& other);
    Number& operator=(Number&&) = default;
    Number(long i);
    Number(long i, unsigned long j);
    ~Number();

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

#endif // Number_H
