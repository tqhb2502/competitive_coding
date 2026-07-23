#include <cstdint>
#include <iostream>

#include "algorithm.hpp"

int main() {
    const std::uint32_t mask =
        cp_library::singleBitOrZero<std::uint32_t>(0) |
        cp_library::singleBitOrZero<std::uint32_t>(2) |
        cp_library::singleBitOrZero<std::uint32_t>(3);

    cp_library::forEachSubmask(mask, [](std::uint32_t submask) {
        std::cout << submask << ' ';
    });
    std::cout << '\n';  // 13 12 9 8 5 4 1 0
    return 0;
}
