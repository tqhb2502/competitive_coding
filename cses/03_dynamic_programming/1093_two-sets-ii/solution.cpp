#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    constexpr int MOD = 1'000'000'007;
    const int total = n * (n + 1) / 2;
    if (total % 2 != 0) {
        std::cout << 0 << '\n';
        return 0;
    }

    const int target = total / 2;
    std::vector<int> dp(target + 1, 0);
    dp[0] = 1;

    // Count subsets of {1, ..., n - 1}. For every valid partition, exactly
    // one of its two sets does not contain n, so no division by two is needed.
    int reachable_sum = 0;
    for (int value = 1; value < n; ++value) {
        reachable_sum += value;
        const int upper = std::min(target, reachable_sum);
        for (int sum = upper; sum >= value; --sum) {
            dp[sum] += dp[sum - value];
            if (dp[sum] >= MOD) {
                dp[sum] -= MOD;
            }
        }
    }

    std::cout << dp[target] << '\n';
    return 0;
}
