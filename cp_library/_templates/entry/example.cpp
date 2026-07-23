#include <iostream>
#include <vector>

#include "algorithm.hpp"

int main() {
    const std::vector<long long> values = {1, 2, 3, 4};
    std::cout << cp_library::sumValues(values) << '\n';
    return 0;
}
