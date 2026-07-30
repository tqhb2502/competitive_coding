#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    const int B = 30;          // x_i <= 10^9 < 2^30 => bit 0..29
    long long basis[B] = {0};  // basis[b]: vector có bit cao nhất đúng bằng b

    for (int i = 0; i < n; ++i) {
        long long x;
        cin >> x;
        // Chèn x vào XOR linear basis: khử dần từ bit cao xuống thấp.
        for (int b = B - 1; b >= 0; --b) {
            if (!((x >> b) & 1LL)) continue;   // bit b của x không bật
            if (!basis[b]) {                   // chưa có vector chiếm bit b
                basis[b] = x;                  // thêm x vào cơ sở
                break;
            }
            x ^= basis[b];                     // khử bit b rồi tiếp tục
        }
        // Nếu x bị khử về 0 nghĩa là phụ thuộc tuyến tính, không thêm gì.
    }

    // Tham lam từ bit cao xuống thấp: bật thêm bit nào làm tăng giá trị XOR.
    long long ans = 0;
    for (int b = B - 1; b >= 0; --b) {
        if ((ans ^ basis[b]) > ans) ans ^= basis[b];
    }

    cout << ans << '\n';
    return 0;
}
