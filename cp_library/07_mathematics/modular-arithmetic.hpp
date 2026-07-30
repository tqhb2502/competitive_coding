#pragma once
#include <bits/stdc++.h>
using namespace std;
// Modular Arithmetic — lũy thừa nhanh theo mod và nghịch đảo modulo (Fermat), O(log e); mod tới ~1e18 nhờ __int128.
// Khi dùng: tính a^e mod m, nghịch đảo modulo khi m NGUYÊN TỐ; nền cho tổ hợp, quy hoạch đếm theo mod.
// ĐPT: O(log e); bộ nhớ O(1). Nhân dùng __int128 nên an toàn với mod tới ~1e18.
// Dùng: modpow(a, e, m); modInv(a, m); // modInv yêu cầu m NGUYÊN TỐ và gcd(a, m) = 1
// Bẫy: modInv dùng Fermat -> m PHẢI nguyên tố (nếu không, dùng extended-euclid); e >= 0; kết quả trong [0, m).
// CSES: 1095 1617 1712 2182 2228 3398
// Ý nghĩa: tính (b^e) mod m bằng bình phương liên tiếp.
// Tham số: b = cơ số; e = số mũ (>= 0); m = modulo.
// Trả về: b^e mod m trong [0, m).
long long modpow(long long b, long long e, long long m) {
    b %= m; if (b < 0) b += m;
    long long r = 1 % m;
    while (e > 0) {
        if (e & 1) r = (__int128)r * b % m;
        b = (__int128)b * b % m;
        e >>= 1;
    }
    return r;
}
// Ý nghĩa: nghịch đảo modulo của a theo m nguyên tố (định lý Fermat nhỏ): a^(m-2) mod m.
// Tham số: a = số cần nghịch đảo; m = modulo NGUYÊN TỐ.
// Trả về: a^{-1} mod m trong [0, m).
long long modInv(long long a, long long m) { return modpow(a, m - 2, m); }

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ modular-arithmetic.hpp -o demo && ./demo
int main() {
    const long long MOD = 1000000007;
    printf("2^10 mod p = %lld ; nghịch đảo 2 mod p = %lld\n", modpow(2, 10, MOD), modInv(2, MOD));   // 1024 ; 500000004
    return 0;
}
#endif
