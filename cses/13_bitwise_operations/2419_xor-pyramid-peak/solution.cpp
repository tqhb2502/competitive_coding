// Xor Pyramid Peak - CSES 2419
// https://cses.fi/problemset/task/2419
//
// Kim tự tháp XOR: mỗi số ở tầng trên là XOR của hai số bên dưới.
// Đỉnh (peak) = XOR của các a_i với C(n-1, i) lẻ.
// Theo định lý Lucas mod 2: C(n-1, i) lẻ  <=>  (i & (n-1)) == i  (i là submask của n-1).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    long long ans = 0;
    int m = n - 1; // hàng có n phần tử, chỉ số 0..n-1, hệ số C(n-1, i)
    for (int i = 0; i < n; ++i) {
        long long a;
        cin >> a;
        // C(n-1, i) lẻ <=> i là submask của (n-1)
        if ((i & m) == i) {
            ans ^= a;
        }
    }

    cout << ans << '\n';
    return 0;
}
