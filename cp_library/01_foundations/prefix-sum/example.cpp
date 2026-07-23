#include <iostream>
#include <vector>

#include "algorithm.hpp"

int main() {
    const std::vector<int> values = {3, -2, 5, 4};
    const cp_library::PrefixSum<long long> sums(values);

    std::cout << sums.query(1, 4) << '\n';
    return 0;
}
