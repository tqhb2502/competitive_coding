#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    constexpr std::uint16_t INF = 10000;
    std::vector<std::uint16_t> distance(static_cast<std::size_t>(n) * k, INF);
    std::array<std::uint16_t, 26> left{};
    std::array<std::uint16_t, 26> diagonal{};

    std::int64_t answer = 0;
    std::string row_data;

    for (int row = 0; row < n; ++row) {
        std::cin >> row_data;
        left.fill(INF);
        diagonal.fill(INF);

        for (int column = 0; column < n; ++column) {
            const int current_letter = row_data[column] - 'A';
            const std::size_t base = static_cast<std::size_t>(column) * k;
            int maximum_distance = 0;

            for (int letter = 0; letter < k; ++letter) {
                const std::uint16_t upper = distance[base + letter];
                std::uint16_t value = 0;

                if (letter != current_letter) {
                    const std::uint16_t best_predecessor =
                        std::min({upper, left[letter], diagonal[letter]});
                    value = static_cast<std::uint16_t>(best_predecessor + 1);
                }

                diagonal[letter] = upper;
                distance[base + letter] = value;
                left[letter] = value;
                maximum_distance = std::max(maximum_distance, static_cast<int>(value));
            }

            const int largest_possible_distance = std::min(row, column);
            if (maximum_distance <= largest_possible_distance) {
                answer += largest_possible_distance - maximum_distance + 1;
            }
        }
    }

    std::cout << answer << '\n';
}
