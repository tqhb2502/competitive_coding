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

    // Đồng xu có giá trị > target không bao giờ dùng được; đồng xu trùng nhau
    // không đổi kết quả nên loại bớt cho gọn (sắp xếp, khử trùng, bỏ phần > target).
    std::sort(coins.begin(), coins.end());
    coins.erase(std::unique(coins.begin(), coins.end()), coins.end());
    coins.erase(std::upper_bound(coins.begin(), coins.end(), target),
                coins.end());

    // dp[s] = số đồng xu ít nhất để tạo tổng s. Khởi tạo INF = target + 1 để
    // đánh dấu "chưa tạo được"; đáp án thật luôn <= target nên không bị nhầm.
    const int unreachable = target + 1;
    std::vector<int> dp(target + 1, unreachable);
    dp[0] = 0;  // Cơ sở: tạo tổng 0 cần 0 đồng xu.

    // Unbounded knapsack: duyệt tổng tăng dần (vòng lặp xuôi) để mỗi đồng xu
    // được dùng lại tùy ý. Chuyển: dp[s] = min(dp[s], dp[s - coin] + 1).
    for (const int coin : coins) {
        for (int sum = coin; sum <= target; ++sum) {
            dp[sum] = std::min(dp[sum], dp[sum - coin] + 1);
        }
    }

    // Nếu dp[target] vẫn là INF thì không tạo được tổng target, in -1.

    std::cout << (dp[target] == unreachable ? -1 : dp[target]) << '\n';
    return 0;
}
