#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    constexpr int MOD = 1'000'000'007;
    std::vector<int> dp(n + 1, 0);
    dp[0] = 1;

    // window is dp[sum - 1] + ... + dp[sum - 6], modulo MOD.
    long long window = 0;
    for (int sum = 1; sum <= n; ++sum) {
        window += dp[sum - 1];
        if (window >= MOD) {
            window -= MOD;
        }
        if (sum >= 7) {
            window -= dp[sum - 7];
            if (window < 0) {
                window += MOD;
            }
        }
        dp[sum] = static_cast<int>(window);
    }

    std::cout << dp[n] << '\n';
    return 0;
}
