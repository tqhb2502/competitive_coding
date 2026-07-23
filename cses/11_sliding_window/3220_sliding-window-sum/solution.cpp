#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    // Tham số của linear congruential generator sinh ra mảng.
    long long x, a, b, c;
    cin >> x >> a >> b >> c;

    // Circular buffer lưu k phần tử gần nhất (giá trị < 10^9 nên int đủ).
    vector<int> buf(k);

    long long sum = 0;   // tổng của cửa sổ hiện tại
    long long ans = 0;   // XOR của tất cả window sum
    long long cur = x;   // x_1 = x

    // Duyệt từng phần tử, sinh trực tiếp trong vòng lặp, không lưu cả mảng.
    for (int i = 0; i < n; ++i) {
        if (i > 0) cur = (a * cur + b) % c;   // x_i = (a*x_{i-1} + b) mod c

        int idx = i % k;
        if (i >= k) sum -= buf[idx];          // phần tử a[i-k] rời cửa sổ
        buf[idx] = (int)cur;                  // ghi đè bằng phần tử mới
        sum += cur;

        // Khi cửa sổ [i-k+1 .. i] đã đủ k phần tử, gom tổng vào đáp án bằng XOR.
        if (i >= k - 1) ans ^= sum;
    }

    cout << ans << '\n';
    return 0;
}
