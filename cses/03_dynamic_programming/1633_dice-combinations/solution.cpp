#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    constexpr int MOD = 1'000'000'007;

    // dp[i] = số cách tạo tổng đúng bằng i; điểm neo dp[0] = 1 (không tung lần nào).
    std::vector<int> dp(n + 1, 0);
    dp[0] = 1;

    // Cửa sổ trượt: window = dp[sum-1] + ... + dp[sum-6] (đã lấy modulo MOD),
    // chính là tổng công thức truy hồi dp[sum] = sum dp[sum-j], j = 1..6.
    long long window = 0;
    for (int sum = 1; sum <= n; ++sum) {
        // Thêm phần tử mới dp[sum-1] vào cửa sổ.
        window += dp[sum - 1];
        if (window >= MOD) {
            window -= MOD;
        }
        // Bỏ phần tử đã ra khỏi cửa sổ (dp[sum-7]) khi cửa sổ vượt quá 6 số hạng.
        if (sum >= 7) {
            window -= dp[sum - 7];
            if (window < 0) {
                window += MOD;
            }
        }
        dp[sum] = static_cast<int>(window);
    }

    // Đáp án là dp[n].
    std::cout << dp[n] << '\n';
    return 0;
}
