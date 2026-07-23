#include <iostream>
#include <vector>

#include "algorithm.hpp"

int main() {
    const std::vector<int> values = {1, 2, 4, 7, 11};

    const auto pair = cp_library::find_pair_with_sum_sorted(values, 9LL);
    if (pair.has_value()) {
        std::cout << pair->first << ' ' << pair->second << '\n';
    }

    std::cout
        << cp_library::count_pairs_with_sum_at_most_sorted(values, 8LL)
        << '\n';
    return 0;
}
