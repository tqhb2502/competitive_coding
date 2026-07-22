// Xor Pyramid Row  (CSES 3195)
// https://cses.fi/problemset/task/3195
//
// Đi lên m = n - k mức từ hàng đáy. Phần tử i của hàng đích là
//   r[i] = XOR_{j submask của m} a[i + j]     (Lucas mod 2: C(m,j) lẻ <=> j submask m)
// Tách theo từng bit bật của m (kiểu SOS): với offset o = 2^p, c[i] ^= c[i+o].
// Độ phức tạp: O(n * popcount(m)) ~ O(n log n).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    int m = n - k;   // số mức phải đi lên
    int len = n;     // số phần tử hợp lệ hiện tại

    for (int p = 0; p < 18; p++) {
        if (m & (1 << p)) {
            int o = 1 << p;
            // Duyệt tăng dần: c[i+o] vẫn là giá trị gốc khi đọc (cập nhật tại chỗ).
            for (int i = 0; i + o < len; i++) a[i] ^= a[i + o];
            len -= o;   // giảm số phần tử hợp lệ
        }
    }
    // len == k tại đây; a[0..k-1] là hàng thứ k tính từ trên xuống.

    string out;
    out.reserve(len * 12);
    for (int i = 0; i < len; i++) {
        if (i) out.push_back(' ');
        out += to_string(a[i]);
    }
    out.push_back('\n');
    fputs(out.c_str(), stdout);

    return 0;
}
