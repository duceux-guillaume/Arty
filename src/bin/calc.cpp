#include <iostream>
#include <limits>

#include <arty/math/core/number.h>

using namespace arty::math;

int main(int argc, char** argv) {
    std::cout << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }

    std::cout << Number(1568792) << std::endl;

    std::cout << Number(10) + Number(850) << std::endl;
    std::cout << std::numeric_limits<int>::max() << std::endl;
    std::cout << Number(std::numeric_limits<int>::max()) + std::numeric_limits<int>::max() << std::endl;
    std::cout << std::numeric_limits<long>::max() << std::endl;
    std::cout << Number(std::numeric_limits<long>::max()) + std::numeric_limits<long>::max() << std::endl;

    Number A(1);
    Number B(2);
    std::cout << A << " - " << B << " = " << A - B << std::endl;

    std::cout << Number(-123456) << std::endl;
    std::cout << Number(123456) << std::endl;

    std::cout << Number(-5000, 100) << std::endl;
    std::cout << Number(-1, 1) << std::endl;

    return 0;
}
