#include <cstddef>
#include <iostream>
#include <vector>

#include "algorithm.hpp"

int main() {
    const std::vector<int> values = {2, 1, 3, 2, 4};
    long long sum = 0;

    cp_library::for_each_fixed_window(
        values.size(),
        3,
        [&](std::size_t index) { sum += values[index]; },
        [&](std::size_t index) { sum -= values[index]; },
        [&](std::size_t, std::size_t) { std::cout << sum << ' '; }
    );
    std::cout << '\n';

    sum = 0;
    cp_library::for_each_longest_valid_suffix(
        values.size(),
        [&](std::size_t index) { sum += values[index]; },
        [&](std::size_t index) { sum -= values[index]; },
        [&]() { return sum <= 5; },
        [&](std::size_t left, std::size_t right) {
            std::cout << '[' << left << ',' << right << ") ";
        }
    );
    std::cout << '\n';
    return 0;
}
