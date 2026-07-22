#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<long long> values(n);
    for (long long& value : values) {
        std::cin >> value;
    }

    const long long total =
        std::accumulate(values.begin(), values.end(), 0LL);

    // dp[left] is the best score difference for the interval beginning at
    // left. Updating left to right keeps both shorter intervals available.
    std::vector<long long> dp = values;
    for (int length = 2; length <= n; ++length) {
        for (int left = 0; left + length <= n; ++left) {
            const int right = left + length - 1;
            dp[left] = std::max(values[left] - dp[left + 1],
                                values[right] - dp[left]);
        }
    }

    const long long first_player_score = (total + dp[0]) / 2;
    std::cout << first_player_score << '\n';
    return 0;
}
