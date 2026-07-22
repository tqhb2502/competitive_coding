#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int MOD = 1'000'000'007;

    int n;
    std::cin >> n;

    std::vector<int> dp(n, 0);
    for (int row = 0; row < n; ++row) {
        std::string cells;
        std::cin >> cells;

        int from_left = 0;
        for (int column = 0; column < n; ++column) {
            if (cells[column] == '*') {
                dp[column] = 0;
                from_left = 0;
            } else if (row == 0 && column == 0) {
                dp[column] = 1;
                from_left = 1;
            } else {
                // dp[column] is the old value from above; from_left is the
                // already updated value immediately to the left.
                int paths = dp[column] + from_left;
                if (paths >= MOD) {
                    paths -= MOD;
                }
                dp[column] = paths;
                from_left = paths;
            }
        }
    }

    std::cout << dp[n - 1] << '\n';
    return 0;
}
