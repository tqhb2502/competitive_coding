#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    constexpr int MOD = 1'000'000'007;
    // total = tổng 1..n; nếu lẻ thì không thể chia đều -> đáp án 0
    const int total = n * (n + 1) / 2;
    if (total % 2 != 0) {
        std::cout << 0 << '\n';
        return 0;
    }

    // Mỗi tập phải có tổng target = total/2; đếm số subset có tổng target
    const int target = total / 2;
    // dp[j] = số subset (mỗi số dùng tối đa 1 lần) có tổng đúng bằng j, lấy mod
    std::vector<int> dp(target + 1, 0);
    dp[0] = 1;  // base case: tập rỗng cho tổng 0

    // Chỉ đếm subset của {1, ..., n-1}: cố định số n ở tập thứ hai nên mỗi cách
    // chia được đếm đúng 1 lần, không cần chia đôi
    int reachable_sum = 0;  // tổng lớn nhất đạt được bằng các item đã xét
    for (int value = 1; value < n; ++value) {
        reachable_sum += value;
        // Cắt tỉa cận trên: không cần vượt quá target lẫn tổng hiện có
        const int upper = std::min(target, reachable_sum);
        // Duyệt sum giảm dần để mỗi item chỉ dùng một lần (0/1 knapsack)
        for (int sum = upper; sum >= value; --sum) {
            dp[sum] += dp[sum - value];
            // Cộng rồi trừ MOD một lần, nhanh hơn phép chia lấy dư
            if (dp[sum] >= MOD) {
                dp[sum] -= MOD;
            }
        }
    }

    std::cout << dp[target] << '\n';
    return 0;
}
