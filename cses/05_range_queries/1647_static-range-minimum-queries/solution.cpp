// Static Range Minimum Queries - CSES 1647
// Sparse table preprocessing, followed by O(1) idempotent minimum queries.

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t q = 0;
    std::cin >> n >> q;

    std::size_t levels = 1;
    while ((std::size_t{1} << levels) <= n) {
        ++levels;
    }

    std::vector<std::vector<int>> table(levels, std::vector<int>(n));
    for (int& value : table[0]) {
        std::cin >> value;
    }
    for (std::size_t level = 1; level < levels; ++level) {
        const std::size_t half = std::size_t{1} << (level - 1);
        const std::size_t length = std::size_t{1} << level;
        for (std::size_t start = 0; start + length <= n; ++start) {
            table[level][start] =
                std::min(table[level - 1][start], table[level - 1][start + half]);
        }
    }

    std::vector<std::size_t> floor_log(n + 1, 0);
    for (std::size_t length = 2; length <= n; ++length) {
        floor_log[length] = floor_log[length / 2] + 1;
    }

    for (std::size_t query = 0; query < q; ++query) {
        std::size_t left = 0;
        std::size_t right = 0;
        std::cin >> left >> right;
        --left;
        --right;
        const std::size_t length = right - left + 1;
        const std::size_t level = floor_log[length];
        const std::size_t block_length = std::size_t{1} << level;
        std::cout << std::min(table[level][left],
                              table[level][right - block_length + 1])
                  << '\n';
    }
    return 0;
}
