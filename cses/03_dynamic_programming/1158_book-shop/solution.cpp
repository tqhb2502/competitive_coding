#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int budget;
    std::cin >> n >> budget;

    std::vector<int> prices(n);
    std::vector<int> pages(n);
    for (int& price : prices) {
        std::cin >> price;
    }
    for (int& page_count : pages) {
        std::cin >> page_count;
    }

    // dp[money] = số trang lớn nhất đạt được khi tổng giá KHÔNG VƯỢT QUÁ money.
    // Base case: chưa xét cuốn nào nên mọi dp[money] = 0.
    std::vector<int> dp(budget + 1, 0);
    for (int book = 0; book < n; ++book) {
        // 0/1 knapsack: duyệt money GIẢM DẦN để mỗi cuốn chỉ được dùng một lần.
        for (int money = budget; money >= prices[book]; --money) {
            dp[money] =
                std::max(dp[money],
                         dp[money - prices[book]] + pages[book]);
        }
    }

    // Đáp án: số trang lớn nhất mua được với ngân sách budget.
    std::cout << dp[budget] << '\n';
    return 0;
}
