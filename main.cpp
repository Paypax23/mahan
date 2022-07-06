
#include "test.hpp"

#include <iostream>

void minimal_example()
{
    kp::Instance instance {
        .items = {{6, 2}, {5, 3}, {8, 6}, {9, 7}, {6, 5}, {7, 9}, {3, 4}},
        .n = 7,
        .capacity = 15}; // X* = {0, 2, 3} value = 23
    auto solution = kp::bellman(instance);
    std::cout << "Minimal example:\n" << instance << '\n' << solution << '\n';
}

int main(int argc, char* argv[])
{
    switch (argc) {
    case 2: kp::test_directory(argv[1]); break;
    default: minimal_example();
    }

    return EXIT_SUCCESS;
}

