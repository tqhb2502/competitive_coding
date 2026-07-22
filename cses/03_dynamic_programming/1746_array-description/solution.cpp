#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr int MOD = 1'000'000'007;

    int n, m;
    cin >> n >> m;
    vector<int> values(n);
    for (int& value : values) {
        cin >> value;
    }

    // Indices 0 and m + 1 are zero padding for the two boundaries.
    vector<int> dp(m + 2, 0), next(m + 2, 0);
    if (values[0] == 0) {
        fill(dp.begin() + 1, dp.begin() + m + 1, 1);
    } else {
        dp[values[0]] = 1;
    }

    for (int index = 1; index < n; ++index) {
        fill(next.begin(), next.end(), 0);
        if (values[index] == 0) {
            for (int value = 1; value <= m; ++value) {
                next[value] = static_cast<int>(
                    (static_cast<long long>(dp[value - 1]) + dp[value] + dp[value + 1]) % MOD);
            }
        } else {
            const int value = values[index];
            next[value] = static_cast<int>(
                (static_cast<long long>(dp[value - 1]) + dp[value] + dp[value + 1]) % MOD);
        }
        dp.swap(next);
    }

    long long answer = 0;
    for (int value = 1; value <= m; ++value) {
        answer += dp[value];
    }
    cout << answer % MOD << '\n';
    return 0;
}
