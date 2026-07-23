#include <bits/stdc++.h>
using namespace std;

int main() {
    const int B = 20;                 // x_i <= 10^6 < 2^20
    const int SZ = 1 << B;            // 1048576
    const int FULL = SZ - 1;

    int n;
    if (scanf("%d", &n) != 1) return 0;

    vector<int> x(n);
    // sub và sup khởi tạo bằng cnt[mask]
    vector<int> sub(SZ, 0), sup(SZ, 0);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x[i]);
        ++sub[x[i]];
        ++sup[x[i]];
    }

    // Subset-sum (SOS DP): sub[mask] = tổng cnt trên mọi submask của mask.
    for (int b = 0; b < B; ++b) {
        int bit = 1 << b;
        for (int mask = 0; mask < SZ; ++mask) {
            if (mask & bit) sub[mask] += sub[mask ^ bit];
        }
    }

    // Superset-sum (SOS DP): sup[mask] = tổng cnt trên mọi supermask của mask.
    for (int b = 0; b < B; ++b) {
        int bit = 1 << b;
        for (int mask = 0; mask < SZ; ++mask) {
            if (!(mask & bit)) sup[mask] += sup[mask | bit];
        }
    }

    // Xuất kết quả, dùng buffer để tăng tốc.
    string out;
    out.reserve((size_t)n * 24);
    char buf[64];
    for (int i = 0; i < n; ++i) {
        int a1 = sub[x[i]];                    // x | y = x
        int a2 = sup[x[i]];                    // x & y = x
        int comp = (~x[i]) & FULL;             // phần bù trong B bit
        int a3 = n - sub[comp];                // x & y != 0
        int len = sprintf(buf, "%d %d %d\n", a1, a2, a3);
        out.append(buf, len);
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
