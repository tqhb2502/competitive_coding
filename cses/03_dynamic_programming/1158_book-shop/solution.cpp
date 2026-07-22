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

    // dp[money] is the largest page count attainable without spending more
    // than money. Descending updates ensure each book is used at most once.
    std::vector<int> dp(budget + 1, 0);
    for (int book = 0; book < n; ++book) {
        for (int money = budget; money >= prices[book]; --money) {
            dp[money] =
                std::max(dp[money],
                         dp[money - prices[book]] + pages[book]);
        }
    }

    std::cout << dp[budget] << '\n';
    return 0;
}
