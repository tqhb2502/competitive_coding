#include <algorithm>
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

    std::vector<int> dp(n, 0);
    std::vector<std::int64_t> answer(k, 0);
    std::string previous(n, '#');
    std::string current;

    for (int row = 0; row < n; ++row) {
        std::cin >> current;
        int left_dp = 0;
        int diagonal_dp = 0;

        for (int column = 0; column < n; ++column) {
            const int upper_dp = dp[column];
            int square_size = 1;

            if (row > 0 && column > 0 && current[column] == previous[column] &&
                current[column] == current[column - 1] &&
                current[column] == previous[column - 1]) {
                square_size = 1 + std::min({upper_dp, left_dp, diagonal_dp});
            }

            dp[column] = square_size;
            left_dp = square_size;
            diagonal_dp = upper_dp;
            answer[current[column] - 'A'] += square_size;
        }

        previous = current;
    }

    for (const std::int64_t count : answer) {
        std::cout << count << '\n';
    }
}
