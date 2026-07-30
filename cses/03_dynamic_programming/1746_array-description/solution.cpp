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

    // dp[v] = số cách điền tiền tố sao cho phần tử cuối mang giá trị v.
    // Chỉ số 0 và m+1 là ô đệm (padding) luôn = 0 để tránh kiểm tra biên.
    vector<int> dp(m + 2, 0), next(m + 2, 0);

    // Cơ sở (i = 0): x[0] == 0 thì mọi v đều được phép, ngược lại chỉ dùng x[0].
    if (values[0] == 0) {
        fill(dp.begin() + 1, dp.begin() + m + 1, 1);
    } else {
        dp[values[0]] = 1;
    }

    // Quét mảng từ trái sang phải theo công thức chuyển trạng thái.
    for (int index = 1; index < n; ++index) {
        fill(next.begin(), next.end(), 0);
        if (values[index] == 0) {
            // Vị trí chưa biết: tính cho mọi giá trị v trong [1, m].
            for (int value = 1; value <= m; ++value) {
                next[value] = static_cast<int>(
                    (static_cast<long long>(dp[value - 1]) + dp[value] + dp[value + 1]) % MOD);
            }
        } else {
            // Vị trí đã biết: chỉ giữ lại đúng giá trị x[index].
            const int value = values[index];
            next[value] = static_cast<int>(
                (static_cast<long long>(dp[value - 1]) + dp[value] + dp[value + 1]) % MOD);
        }
        dp.swap(next);
    }

    // Đáp án: tổng dp[v] với v từ 1 tới m (lấy modulo).
    long long answer = 0;
    for (int value = 1; value <= m; ++value) {
        answer += dp[value];
    }
    cout << answer % MOD << '\n';
    return 0;
}
