#pragma once
#include <bits/stdc++.h>
using namespace std;
// Burnside (đếm chuỗi vòng) — bổ đề Burnside cho nhóm QUAY: số chuỗi vòng độ dài n, k màu = (1/n)·Σ_{i=0..n-1} k^gcd(i,n).
// Khi dùng: đếm cấu hình trên vòng tròn mà QUAY được coi là giống nhau (vòng cổ, tô màu đỉnh đa giác, xâu tuần hoàn).
// ĐPT: countNecklaces/countBracelets O(sqrt(n) + d(n)·log n) (phân tích n + duyệt ước + lũy thừa nhanh); bộ nhớ O(log n).
// Dùng: countNecklaces(4, 2, 1000000007);  // 6 (chỉ xét quay)   |   countBracelets(6, 2, 1000000007);  // 13 (quay + phản xạ)
// Bẫy: mod phải NGUYÊN TỐ và gcd(n, mod) = 1 để có nghịch đảo của n (mod nguyên tố > n là đủ; countBracelets cần thêm mod lẻ);
//      n >= 0, k >= 0 (n = 0 trả 1 — chuỗi rỗng); công thức nhóm nhị diện D_n = (1/(2n))·[Σ_{i} k^gcd(i,n) + PHẢN_XẠ], với
//      PHẢN_XẠ = n·k^((n+1)/2) khi n LẺ, và (n/2)·(k^(n/2) + k^(n/2+1)) khi n CHẴN (n/2 trục qua 2 đỉnh, n/2 trục qua 2 cạnh).
//      Chỉ có sẵn nhóm QUAY/nhị diện trên VÒNG; nhóm khác phải TỰ đếm điểm bất động của từng phép (vd CSES 2210 đếm
//      bảng n×n với nhóm C4 = 4 phép xoay: cùng bổ đề nhưng số bất động khác — xem cses/07_mathematics/2210_counting-grids).
// CSES: 2209 2210
// Ý nghĩa: đếm số chuỗi vòng độ dài n với k màu, hai chuỗi coi là GIỐNG nhau nếu quay được thành nhau (bổ đề Burnside).
//          Rút gọn Σ_{i=0..n-1} k^gcd(i,n) = Σ_{d | n} phi(n/d)·k^d để chạy O(sqrt(n)) thay vì O(n).
// Tham số: n = độ dài vòng (>= 0); k = số màu (>= 0); mod = modulo NGUYÊN TỐ, không chia hết n.
// Trả về: số lớp tương đương theo phép quay, lấy dư mod, trong [0, mod).
long long countNecklaces(long long n, long long k, long long mod) {
    if (mod <= 1) return 0;
    if (n <= 0) return 1 % mod;
    // Lũy thừa nhanh theo mod, nhân qua __int128 nên an toàn với mod tới ~1e18.
    auto power = [&](long long b, long long e) {
        b %= mod; if (b < 0) b += mod;
        long long r = 1 % mod;
        while (e > 0) {
            if (e & 1) r = (__int128)r * b % mod;
            b = (__int128)b * b % mod;
            e >>= 1;
        }
        return r;
    };
    // Các nguyên tố của n: mọi ước của n chỉ gồm những nguyên tố này nên tính phi rất nhanh.
    vector<long long> primes;
    for (long long p = 2, t = n; ; p++) {
        if (p * p > t) { if (t > 1) primes.push_back(t); break; }
        if (t % p == 0) { primes.push_back(p); while (t % p == 0) t /= p; }
    }
    // Hàm Euler phi(x) cho x là ước của n (chỉ cần thử các nguyên tố của n).
    auto phi = [&](long long x) {
        long long res = x;
        for (long long p : primes) if (x % p == 0) res -= res / p;
        return res;
    };
    long long sum = 0;
    for (long long d = 1; d * d <= n; d++) {
        if (n % d != 0) continue;
        long long e = n / d;                       // cặp ước (d, e = n/d)
        sum = (sum + (__int128)(phi(e) % mod) * power(k, d)) % mod;
        if (d != e) sum = (sum + (__int128)(phi(d) % mod) * power(k, e)) % mod;
    }
    return (__int128)sum * power(n % mod, mod - 2) % mod;   // chia n bằng nghịch đảo Fermat
}
// Ý nghĩa: đếm số vòng tay (bracelet) độ dài n với k màu dưới nhóm NHỊ DIỆN D_n (quay VÀ phản xạ):
//          (1/(2n))·[Σ_{i=0..n-1} k^gcd(i,n) + PHẢN_XẠ], PHẢN_XẠ = n·k^((n+1)/2) nếu n lẻ, (n/2)·(k^(n/2)+k^(n/2+1)) nếu n chẵn.
// Tham số: n = độ dài vòng (>= 0); k = số màu (>= 0); mod = modulo NGUYÊN TỐ LẺ, không chia hết n.
// Trả về: số lớp tương đương theo quay + phản xạ, lấy dư mod, trong [0, mod).
long long countBracelets(long long n, long long k, long long mod) {
    if (mod <= 1) return 0;
    if (n <= 0) return 1 % mod;
    auto power = [&](long long b, long long e) {
        b %= mod; if (b < 0) b += mod;
        long long r = 1 % mod;
        while (e > 0) {
            if (e & 1) r = (__int128)r * b % mod;
            b = (__int128)b * b % mod;
            e >>= 1;
        }
        return r;
    };
    long long rot = (__int128)countNecklaces(n, k, mod) * (n % mod) % mod;   // lấy lại Σ_i k^gcd(i,n)
    long long refl;
    if (n % 2 == 1) refl = (__int128)(n % mod) * power(k, (n + 1) / 2) % mod;
    else refl = (__int128)((n / 2) % mod) * ((power(k, n / 2) + power(k, n / 2 + 1)) % mod) % mod;
    long long total = (rot + refl) % mod;
    return (__int128)total * power(2 * (n % mod) % mod, mod - 2) % mod;      // chia 2n
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ burnside.hpp -o demo && ./demo
int main() {
    const long long MOD = 1000000007;
    printf("necklaces(4,2)=%lld ; necklaces(3,3)=%lld ; necklaces(6,2)=%lld\n",
           countNecklaces(4, 2, MOD), countNecklaces(3, 3, MOD), countNecklaces(6, 2, MOD));   // 6 ; 11 ; 14
    printf("bracelets(4,2)=%lld ; bracelets(6,2)=%lld\n",
           countBracelets(4, 2, MOD), countBracelets(6, 2, MOD));                              // 6 ; 13
    printf("necklaces(1e9,1e9)=%lld\n", countNecklaces(1000000000LL, 1000000000LL, MOD));      // 898487047 (CSES 2209)
    return 0;
}
#endif
