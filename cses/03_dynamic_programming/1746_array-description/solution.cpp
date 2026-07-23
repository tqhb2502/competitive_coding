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

    // dp[v] = so cach dien tien to sao cho phan tu cuoi mang gia tri v.
    // Chi so 0 va m+1 la o dem (padding) luon = 0 de tranh kiem tra bien.
    vector<int> dp(m + 2, 0), next(m + 2, 0);

    // Co so (i = 0): x[0] == 0 thi moi v deu duoc phep, nguoc lai chi dung x[0].
    if (values[0] == 0) {
        fill(dp.begin() + 1, dp.begin() + m + 1, 1);
    } else {
        dp[values[0]] = 1;
    }

    // Quet mang tu trai sang phai theo cong thuc chuyen trang thai.
    for (int index = 1; index < n; ++index) {
        fill(next.begin(), next.end(), 0);
        if (values[index] == 0) {
            // Vi tri chua biet: tinh cho moi gia tri v trong [1, m].
            for (int value = 1; value <= m; ++value) {
                next[value] = static_cast<int>(
                    (static_cast<long long>(dp[value - 1]) + dp[value] + dp[value + 1]) % MOD);
            }
        } else {
            // Vi tri da biet: chi giu lai dung gia tri x[index].
            const int value = values[index];
            next[value] = static_cast<int>(
                (static_cast<long long>(dp[value - 1]) + dp[value] + dp[value + 1]) % MOD);
        }
        dp.swap(next);
    }

    // Dap an: tong dp[v] voi v tu 1 toi m (lay modulo).
    long long answer = 0;
    for (int value = 1; value <= m; ++value) {
        answer += dp[value];
    }
    cout << answer % MOD << '\n';
    return 0;
}
