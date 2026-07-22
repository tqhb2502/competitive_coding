// Sliding Window Sum - CSES 3220
// https://cses.fi/problemset/task/3220
//
// Sinh mảng bằng linear congruential generator, dùng sliding window với
// running sum + circular buffer kích thước k, XOR tất cả các window sum.
// Thời gian O(n), bộ nhớ O(k).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    long long x, a, b, c;
    cin >> x >> a >> b >> c;

    // Circular buffer lưu k phần tử gần nhất (giá trị < 10^9 nên int đủ).
    vector<int> buf(k);

    long long sum = 0;   // tổng của cửa sổ hiện tại
    long long ans = 0;   // XOR của tất cả window sum
    long long cur = x;   // x_1 = x

    for (int i = 0; i < n; ++i) {
        if (i > 0) cur = (a * cur + b) % c;   // x_i = (a*x_{i-1} + b) mod c

        int idx = i % k;
        if (i >= k) sum -= buf[idx];          // phần tử a[i-k] rời cửa sổ
        buf[idx] = (int)cur;
        sum += cur;

        if (i >= k - 1) ans ^= sum;           // cửa sổ [i-k+1 .. i] đã đủ k phần tử
    }

    cout << ans << '\n';
    return 0;
}
