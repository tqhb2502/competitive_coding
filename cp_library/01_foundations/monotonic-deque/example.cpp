#include <cstddef>
#include <iostream>
#include <vector>

#include "algorithm.hpp"

int main() {
    const std::vector<int> values = {4, 2, 2, 5, 1, 3};

    const auto minimums =
        cp_library::sliding_window_min_values(values, 3);
    for (const int value : minimums) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    const auto maximum_indices =
        cp_library::sliding_window_max_indices(values, 3);
    for (const std::size_t index : maximum_indices) {
        std::cout << index << ' ';
    }
    std::cout << '\n';
    return 0;
}
