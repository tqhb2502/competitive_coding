#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int target;
    std::cin >> n >> target;

    std::vector<int> coins(n);
    for (int& coin : coins) {
        std::cin >> coin;
    }

    // Values larger than target can never be used. Repeated denominations do
    // not change a minimum, so remove them as in the Python solution.
    std::sort(coins.begin(), coins.end());
    coins.erase(std::unique(coins.begin(), coins.end()), coins.end());
    coins.erase(std::upper_bound(coins.begin(), coins.end(), target),
                coins.end());

    const int unreachable = target + 1;
    std::vector<int> dp(target + 1, unreachable);
    dp[0] = 0;

    // Increasing sums allow each denomination to be used any number of times.
    for (const int coin : coins) {
        for (int sum = coin; sum <= target; ++sum) {
            dp[sum] = std::min(dp[sum], dp[sum - coin] + 1);
        }
    }

    std::cout << (dp[target] == unreachable ? -1 : dp[target]) << '\n';
    return 0;
}
