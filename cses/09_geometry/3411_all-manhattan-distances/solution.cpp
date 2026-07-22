#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using WideInteger = __int128_t;

WideInteger sum_pairwise_distances(std::vector<std::int64_t>& values) {
    std::sort(values.begin(), values.end());
    WideInteger result = 0;
    WideInteger prefix_sum = 0;
    for (std::size_t index = 0; index < values.size(); ++index) {
        const WideInteger value = values[index];
        result += value * static_cast<WideInteger>(index) - prefix_sum;
        prefix_sum += value;
    }
    return result;
}

void print_wide_integer(WideInteger value) {
    if (value == 0) {
        std::cout << "0\n";
        return;
    }

    std::string digits;
    while (value > 0) {
        digits.push_back(static_cast<char>('0' + value % 10));
        value /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    std::cout << digits << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int point_count = 0;
    std::cin >> point_count;
    std::vector<std::int64_t> xs(static_cast<std::size_t>(point_count));
    std::vector<std::int64_t> ys(static_cast<std::size_t>(point_count));
    for (int i = 0; i < point_count; ++i) {
        std::cin >> xs[static_cast<std::size_t>(i)] >> ys[static_cast<std::size_t>(i)];
    }

    print_wide_integer(sum_pairwise_distances(xs) + sum_pairwise_distances(ys));
    return 0;
}
