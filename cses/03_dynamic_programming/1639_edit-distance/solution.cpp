#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string source;
    std::string target;
    std::cin >> source >> target;

    // Use the shorter string as the columns of the rolling DP row.
    if (source.size() < target.size()) {
        std::swap(source, target);
    }

    const int rows = static_cast<int>(source.size());
    const int columns = static_cast<int>(target.size());
    std::vector<int> dp(columns + 1);
    std::iota(dp.begin(), dp.end(), 0);

    for (int row = 1; row <= rows; ++row) {
        int diagonal = dp[0];
        dp[0] = row;

        for (int column = 1; column <= columns; ++column) {
            const int from_above = dp[column];
            if (source[row - 1] == target[column - 1]) {
                dp[column] = diagonal;
            } else {
                dp[column] = 1 + std::min({from_above,
                                           dp[column - 1],
                                           diagonal});
            }
            diagonal = from_above;
        }
    }

    std::cout << dp[columns] << '\n';
    return 0;
}
