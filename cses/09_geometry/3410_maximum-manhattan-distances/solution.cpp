#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int point_count = 0;
    std::cin >> point_count;

    std::int64_t minimum_sum = std::numeric_limits<std::int64_t>::max();
    std::int64_t maximum_sum = std::numeric_limits<std::int64_t>::min();
    std::int64_t minimum_difference = std::numeric_limits<std::int64_t>::max();
    std::int64_t maximum_difference = std::numeric_limits<std::int64_t>::min();

    for (int i = 0; i < point_count; ++i) {
        std::int64_t x = 0;
        std::int64_t y = 0;
        std::cin >> x >> y;

        const std::int64_t sum = x + y;
        const std::int64_t difference = x - y;
        minimum_sum = std::min(minimum_sum, sum);
        maximum_sum = std::max(maximum_sum, sum);
        minimum_difference = std::min(minimum_difference, difference);
        maximum_difference = std::max(maximum_difference, difference);

        std::cout << std::max(maximum_sum - minimum_sum,
                              maximum_difference - minimum_difference)
                  << '\n';
    }
    return 0;
}
