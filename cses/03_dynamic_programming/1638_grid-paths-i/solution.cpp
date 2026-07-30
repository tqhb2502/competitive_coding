#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int MOD = 1'000'000'007;

    int n;
    std::cin >> n;

    // dp[j] = số đường đi đến ô (row, j) của hàng hiện tại (mảng cuộn 1 chiều).
    std::vector<int> dp(n, 0);
    for (int row = 0; row < n; ++row) {
        std::string cells;
        std::cin >> cells;

        // from_left giữ dp[column-1] vừa tính của hàng này; đầu hàng chưa có ô trái nên = 0.
        int from_left = 0;
        for (int column = 0; column < n; ++column) {
            if (cells[column] == '*') {
                // Ô bẫy: không đứng được, cũng chặn đường đi qua nó.
                dp[column] = 0;
                from_left = 0;
            } else if (row == 0 && column == 0) {
                // Cơ sở: ô xuất phát.
                dp[column] = 1;
                from_left = 1;
            } else {
                // dp[column] đang là giá trị hàng trên; from_left là ô bên trái cùng hàng.
                int paths = dp[column] + from_left;
                if (paths >= MOD) {  // trừ nhanh thay cho phép chia lấy dư
                    paths -= MOD;
                }
                dp[column] = paths;
                from_left = paths;
            }
        }
    }

    // Đáp án: số đường đi đến ô dưới-phải.
    std::cout << dp[n - 1] << '\n';
    return 0;
}
