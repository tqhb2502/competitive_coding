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
    // Sắp xếp tăng dần để vòng trong có thể break sớm khi coin > sum.
    std::sort(coins.begin(), coins.end());

    // dp[s] = số cách CÓ THỨ TỰ tạo ra tổng đúng bằng s; cơ sở dp[0] = 1.
    std::vector<int> dp(target + 1, 0);
    dp[0] = 1;

    // Vòng ngoài duyệt tổng s, vòng trong cố định đồng xu ĐỨNG CUỐI của dãy là
    // coin -> dp[s] = tổng dp[s - coin]. Nhờ vòng ngoài theo s mà các dãy khác
    // thứ tự được đếm tách biệt (thứ tự quan trọng).
    for (int sum = 1; sum <= target; ++sum) {
        int ways = 0;
        for (const int coin : coins) {
            // coins đã sắp xếp tăng dần nên gặp coin > sum là break sớm.
            if (coin > sum) {
                break;
            }
            ways += dp[sum - coin];
            // Trừ MOD thay cho phép chia lấy dư để giữ ways trong [0, MOD).
            if (ways >= MOD) {
                ways -= MOD;
            }
        }
        dp[sum] = ways;
    }

    std::cout << dp[target] << '\n';
    return 0;
}
