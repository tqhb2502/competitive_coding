#include <iostream>
#include <vector>

#include "algorithm.hpp"

int main() {
    const std::vector<long long> machine_times = {2, 3, 7};
    const long long products_needed = 7;

    const auto enough_time = [&](long long time) {
        long long produced = 0;
        for (const long long machine_time : machine_times) {
            produced += time / machine_time;
            if (produced >= products_needed) {
                return true;
            }
        }
        return false;
    };

    // 0 chắc chắn false, 10^18 chắc chắn true với dữ liệu này.
    const long long answer =
        cp_library::binarySearchFirstTrue(0LL, 1'000'000'000'000'000'000LL,
                                          enough_time);
    std::cout << answer << '\n';  // 8
    return 0;
}
