// Xor Pyramid Diagonal - CSES 3194
// https://cses.fi/problemset/task/3194
//
// leftmost(dòng k) = XOR của các a[i] với C(k,i) lẻ.
// Theo Lucas mod 2: C(k,i) lẻ <=> i là submask của k.
// => leftmost(k) = XOR của a[j] trên mọi submask j của k  ->  SOS (XOR over subsets) DP.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int B = 0;
    while ((1 << B) < n) ++B;          // 2^B >= n, B <= 18 với n <= 2e5
    int SZ = 1 << B;

    vector<int> f(SZ, 0);
    for (int i = 0; i < n; ++i) scanf("%d", &f[i]);

    // SOS DP: sau vòng lặp, f[mask] = XOR a[j] trên mọi submask j của mask.
    for (int bit = 0; bit < B; ++bit) {
        int b = 1 << bit;
        for (int mask = 0; mask < SZ; ++mask) {
            if (mask & b) f[mask] ^= f[mask ^ b];
        }
    }

    // Đáp án cho dòng k (0 = dòng đáy) là f[k]; in từ dưới lên trên = f[0..n-1].
    for (int k = 0; k < n; ++k) {
        printf("%d%c", f[k], k + 1 < n ? ' ' : '\n');
    }
    return 0;
}
