#include <iostream>
#include <vector>

#include "algorithm.hpp"

int main() {
    const std::vector<long long> coordinates = {
        1000000000LL, -7, 1000000000LL, 42
    };
    const cp_library::CoordinateCompression<long long> compression(coordinates);

    for (const std::size_t rank : compression.compress(coordinates)) {
        std::cout << rank << ' ';
    }
    std::cout << '\n';
    return 0;
}
