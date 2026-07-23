#include <cstddef>
#include <iostream>
#include <vector>

#include "algorithm.hpp"

namespace {

void printIndices(const std::vector<std::size_t>& indices) {
    const std::size_t sentinel = indices.size();
    for (const std::size_t index : indices) {
        if (index == sentinel) {
            std::cout << -1 << ' ';
        } else {
            std::cout << index << ' ';
        }
    }
    std::cout << '\n';
}

}  // namespace

int main() {
    const std::vector<int> values = {2, 3, 3, 1};

    printIndices(cp_library::nearestPreviousLess(
        values, cp_library::MonotonicStrictness::Strict));
    printIndices(cp_library::nearestPreviousLess(
        values, cp_library::MonotonicStrictness::NonStrict));

    // Strict:    -1 0 0 -1
    // NonStrict: -1 0 1 -1
    return 0;
}
