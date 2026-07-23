#include <cassert>
#include <vector>

#include "algorithm.hpp"

int main() {
    const std::vector<long long> empty;
    const std::vector<long long> positive = {1, 2, 3};
    const std::vector<long long> mixed = {-5, 2, 9};

    assert(cp_library::sumValues(empty) == 0);
    assert(cp_library::sumValues(positive) == 6);
    assert(cp_library::sumValues(mixed) == 6);
    return 0;
}
