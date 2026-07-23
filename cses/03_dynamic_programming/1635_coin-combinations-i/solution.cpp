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
    // Sap xep tang dan de vong trong co the break som khi coin > sum.
    std::sort(coins.begin(), coins.end());

    // dp[s] = so cach CO THU TU tao ra tong dung bang s; co so dp[0] = 1.
    std::vector<int> dp(target + 1, 0);
    dp[0] = 1;

    // Vong ngoai duyet tong s, vong trong co dinh dong xu DUNG CUOI cua day la
    // coin -> dp[s] = tong dp[s - coin]. Nho vong ngoai theo s ma cac day khac
    // thu tu duoc dem tach biet (thu tu quan trong).
    for (int sum = 1; sum <= target; ++sum) {
        int ways = 0;
        for (const int coin : coins) {
            // coins da sap xep tang dan nen gap coin > sum la break som.
            if (coin > sum) {
                break;
            }
            ways += dp[sum - coin];
            // Tru MOD thay cho phep chia lay du de giu ways trong [0, MOD).
            if (ways >= MOD) {
                ways -= MOD;
            }
        }
        dp[sum] = ways;
    }

    std::cout << dp[target] << '\n';
    return 0;
}
