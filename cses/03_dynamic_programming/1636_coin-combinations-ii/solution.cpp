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

    std::vector<int> ways(target + 1, 0);
    ways[0] = 1;

    // Process one denomination at a time so every multiset is generated once.
    // Increasing sums still allow the current coin to be used repeatedly.
    for (const int coin : coins) {
        if (coin > target) {
            continue;
        }
        for (int sum = coin; sum <= target; ++sum) {
            ways[sum] += ways[sum - coin];
            if (ways[sum] >= MOD) {
                ways[sum] -= MOD;
            }
        }
    }

    std::cout << ways[target] << '\n';
    return 0;
}
