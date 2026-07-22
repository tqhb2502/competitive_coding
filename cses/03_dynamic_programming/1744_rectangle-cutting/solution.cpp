#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b;
    cin >> a >> b;

    const int limit = max(a, b);
    vector<vector<int>> dp(limit + 1, vector<int>(limit + 1, 0));

    // Compute only i < j, then mirror because dp[i][j] = dp[j][i].
    for (int i = 1; i <= limit; ++i) {
        for (int j = i + 1; j <= limit; ++j) {
            int best = numeric_limits<int>::max();

            // Split the side of length j. Symmetric cut positions are equal.
            for (int cut = 1; cut <= j / 2; ++cut) {
                best = min(best, dp[i][cut] + dp[i][j - cut] + 1);
            }

            // Split the side of length i.
            for (int cut = 1; cut <= i / 2; ++cut) {
                best = min(best, dp[j][cut] + dp[j][i - cut] + 1);
            }

            dp[i][j] = best;
            dp[j][i] = best;
        }
    }

    cout << dp[a][b] << '\n';
    return 0;
}
