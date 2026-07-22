#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int MOD = 1'000'000'007;

    int n;
    int target;
    std::cin >> n >> target;

    std::vector<int> coins(n);
    for (int& coin : coins) {
        std::cin >> coin;
    }
    std::sort(coins.begin(), coins.end());

    std::vector<int> dp(target + 1, 0);
    dp[0] = 1;

    // Fix the last coin of the sequence. Keeping sum in the outer loop is
    // what makes differently ordered sequences count as different ways.
    for (int sum = 1; sum <= target; ++sum) {
        int ways = 0;
        for (const int coin : coins) {
            if (coin > sum) {
                break;
            }
            ways += dp[sum - coin];
            if (ways >= MOD) {
                ways -= MOD;
            }
        }
        dp[sum] = ways;
    }

    std::cout << dp[target] << '\n';
    return 0;
}
