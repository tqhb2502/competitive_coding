#pragma once
#include <bits/stdc++.h>
using namespace std;
// Combinatorics — tiền xử lý giai thừa + nghịch đảo giai thừa để tính C(n, r), P(n, r) mod NGUYÊN TỐ, O(1) mỗi truy vấn.
// Khi dùng: nhiều truy vấn tổ hợp/chỉnh hợp theo mod nguyên tố; Catalan, stars-and-bars, hoán vị lặp.
// ĐPT: dựng O(n), C/P O(1); bộ nhớ O(n). mod phải NGUYÊN TỐ và > n. Nhân dùng __int128 nên mod tới ~1e18 an toàn.
// Dùng: Comb cb(n, MOD); cb.C(n, r); cb.P(n, r); // C = tổ hợp, P = chỉnh hợp
// Bẫy: mod PHẢI nguyên tố; dựng với n đủ lớn (>= mọi n truy vấn); C trả 0 khi r < 0 hoặc r > n.
// CSES: 1072 1078 1079 1715 1716 2064 2176 2187 2228 2429 3232 3400 3421
struct Comb {
    long long mod;
    vector<long long> f, invf;
    // Ý nghĩa: tiền xử lý giai thừa f[i] và nghịch đảo giai thừa invf[i] tới n theo mod nguyên tố.
    // Tham số: n = giới hạn trên của đối số; mod = modulo NGUYÊN TỐ.
    Comb(int n, long long mod) : mod(mod), f(n + 1), invf(n + 1) {
        auto mpow = [&](long long b, long long e) {
            long long r = 1 % mod; b %= mod;
            while (e) { if (e & 1) r = (__int128)r * b % mod; b = (__int128)b * b % mod; e >>= 1; }
            return r;
        };
        f[0] = 1;
        for (int i = 1; i <= n; i++) f[i] = (__int128)f[i - 1] * i % mod;
        invf[n] = mpow(f[n], mod - 2);
        for (int i = n; i > 0; i--) invf[i - 1] = (__int128)invf[i] * i % mod;
    }
    // Ý nghĩa: số tổ hợp chập r của n phần tử, C(n, r) mod.
    // Tham số: n, r = đối số tổ hợp.
    // Trả về: C(n, r) mod, hoặc 0 nếu r < 0 hay r > n.
    long long C(int n, int r) const {
        if (r < 0 || r > n) return 0;
        __int128 x = (__int128)f[n] * invf[r] % mod;
        return (long long)(x * invf[n - r] % mod);
    }
    // Ý nghĩa: số chỉnh hợp chập r của n phần tử, P(n, r) mod.
    // Tham số: n, r = đối số chỉnh hợp.
    // Trả về: P(n, r) mod, hoặc 0 nếu r < 0 hay r > n.
    long long P(int n, int r) const {
        if (r < 0 || r > n) return 0;
        return (long long)((__int128)f[n] * invf[n - r] % mod);
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ combinatorics.hpp -o demo && ./demo
int main() {
    Comb cb(10, 1000000007);
    printf("C(5,2) = %lld ; C(10,3) = %lld ; P(5,2) = %lld\n", cb.C(5, 2), cb.C(10, 3), cb.P(5, 2));   // 10 ; 120 ; 20
    return 0;
}
#endif
