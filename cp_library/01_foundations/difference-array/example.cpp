#include <iostream>
#include <vector>

#include "algorithm.hpp"

int main() {
    cp_library::DifferenceArray<long long> updates(
        std::vector<long long>{2, 2, 2, 2, 2}
    );
    updates.add(1, 4, 3);
    updates.add(3, 5, -1);

    for (const long long value : updates.materialize()) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
    return 0;
}
