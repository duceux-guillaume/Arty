#include <iostream>
#include <limits>

#include <arty/math/core/number.h>

using namespace arty::math;

int main(int argc, char** argv) {
    std::cout << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }

    std::cout << Whole(1568792) << std::endl;

    std::cout << Whole(10) + Whole(850) << std::endl;
    std::cout << std::numeric_limits<int>::max() << std::endl;
    std::cout << Whole(std::numeric_limits<int>::max()) + std::numeric_limits<int>::max() << std::endl;
    std::cout << std::numeric_limits<long>::max() << std::endl;
    std::cout << Whole(std::numeric_limits<long>::max()) + std::numeric_limits<long>::max() << std::endl;

    Whole A(1);
    Whole B(2);
    std::cout << A << " - " << B << " = " << A - B << std::endl;

    std::cout << Integer(-123456) << std::endl;
    std::cout << Integer(123456) << std::endl;

    return 0;
}
