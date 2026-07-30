#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    // Đỉnh kim tự tháp = XOR của các a_i mà hệ số nhị thức C(n-1, i) là số lẻ.
    // Theo định lý Lucas mod 2: C(n-1, i) lẻ  <=>  (i & (n-1)) == i
    // (tức i là submask của n-1, các bit đã bật của i nằm trong các bit đã bật của n-1).
    long long ans = 0;
    int m = n - 1;                 // hàng có n phần tử, chỉ số 0..n-1, hệ số C(n-1, i)
    for (int i = 0; i < n; ++i) {
        long long a;
        cin >> a;
        if ((i & m) == i) {        // i là submask của (n-1) -> hệ số lẻ, đưa vào tổng XOR
            ans ^= a;
        }
    }

    cout << ans << '\n';
    return 0;
}
