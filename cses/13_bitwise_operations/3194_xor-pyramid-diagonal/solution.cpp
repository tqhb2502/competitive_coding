#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    // Chọn B nhỏ nhất sao cho 2^B >= n (B <= 18 với n <= 2e5).
    int B = 0;
    while ((1 << B) < n) ++B;
    int SZ = 1 << B;

    // f[mask] khởi tạo bằng a[mask] nếu mask < n, các mask còn lại để 0.
    vector<int> f(SZ, 0);
    for (int i = 0; i < n; ++i) scanf("%d", &f[i]);

    // SOS DP: sau vòng lặp, f[mask] = XOR các a[j] trên mọi submask j của mask.
    // Đây chính là leftmost(mask) theo định lý Lucas mod 2 (C(k,i) lẻ <=> i submask k).
    for (int bit = 0; bit < B; ++bit) {
        int b = 1 << bit;
        for (int mask = 0; mask < SZ; ++mask) {
            if (mask & b) f[mask] ^= f[mask ^ b];
        }
    }

    // Đáp án dòng k (k = 0 là dòng đáy) là f[k]; in từ dưới lên trên = f[0..n-1].
    for (int k = 0; k < n; ++k) {
        printf("%d%c", f[k], k + 1 < n ? ' ' : '\n');
    }
    return 0;
}
