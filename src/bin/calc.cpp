#include <iostream>

#include <arty/math/core/number.h>

using namespace arty::math;

int main(int argc, char** argv) {
    std::string input;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            input.append(argv[i]);
        }
    }

    std::cout << input << std::endl;

    return 0;
}
