#pragma once
#include <bits/stdc++.h>
using namespace std;
// Factorization — kiểm tra nguyên tố (Miller-Rabin xác định 64-bit) + phân tích thừa số (Pollard-Rho), mọi n < 2^64.
// Khi dùng: n QUÁ LỚN để sàng (tới ~1.8e19): kiểm nguyên tố, phân tích thừa số, đếm/tổng ước của số lớn.
// ĐPT: millerRabin O(log^2 n); factorize ~O(n^{1/4}) mỗi thừa số; bộ nhớ O(số thừa số). Nhân dùng UNSIGNED __int128.
// Dùng: millerRabin(n); auto fs = factorize(n); // fs = danh sách thừa số nguyên tố (có lặp), đã sắp tăng dần
// Bẫy: n là unsigned long long (< 2^64); Miller-Rabin ở đây XÁC ĐỊNH (bộ cơ số cố định) cho mọi n 64-bit.
// CSES: 3169 3396 3406
// Ý nghĩa: nhân hai số 64-bit theo mod bằng UNSIGNED __int128 (đúng cho toàn dải 64-bit, không tràn).
// Tham số: a, b = hai thừa số; m = modulo.
// Trả về: a*b mod m.
unsigned long long mulmod64(unsigned long long a, unsigned long long b, unsigned long long m) {
    return (unsigned __int128)a * b % m;
}
// Ý nghĩa: lũy thừa 64-bit theo mod bằng bình phương liên tiếp.
// Tham số: b = cơ số; e = số mũ; m = modulo.
// Trả về: b^e mod m.
unsigned long long powmod64(unsigned long long b, unsigned long long e, unsigned long long m) {
    unsigned long long r = 1 % m; b %= m;
    while (e) { if (e & 1) r = mulmod64(r, b, m); b = mulmod64(b, b, m); e >>= 1; }
    return r;
}
// Ý nghĩa: kiểm tra n có phải số nguyên tố bằng Miller-Rabin xác định (bộ cơ số phủ mọi n 64-bit).
// Tham số: n = số cần kiểm tra.
// Trả về: true nếu n là số nguyên tố.
bool millerRabin(unsigned long long n) {
    if (n < 2) return false;
    for (unsigned long long p : {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull, 37ull})
        if (n % p == 0) return n == p;
    unsigned long long d = n - 1; int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }
    for (unsigned long long a : {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull, 37ull}) {
        unsigned long long x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int i = 0; i < r - 1; i++) { x = mulmod64(x, x, n); if (x == n - 1) { composite = false; break; } }
        if (composite) return false;
    }
    return true;
}
// Ý nghĩa: tìm MỘT ước không tầm thường của n hợp số bằng Pollard-Rho (thử đổi hằng c tới khi thành công).
// Tham số: n = số hợp cần tách (n chẵn trả 2).
// Trả về: một ước d với 1 < d < n.
unsigned long long pollardRho(unsigned long long n) {
    if (n % 2 == 0) return 2;
    for (unsigned long long c = 1;; c++) {
        unsigned long long x = 2, y = 2, d = 1;
        auto f = [&](unsigned long long v) { return (mulmod64(v, v, n) + c) % n; };
        while (d == 1) { x = f(x); y = f(f(y)); d = std::gcd(x > y ? x - y : y - x, n); }
        if (d != n) return d;
    }
}
// Ý nghĩa: phân tích n thành các thừa số nguyên tố (Miller-Rabin phân biệt nguyên tố / hợp, Pollard-Rho để tách).
// Tham số: n = số cần phân tích (n >= 1).
// Trả về: danh sách thừa số nguyên tố (có lặp), đã sắp tăng dần.
vector<unsigned long long> factorize(unsigned long long n) {
    vector<unsigned long long> out;
    function<void(unsigned long long)> rec = [&](unsigned long long m) {
        if (m == 1) return;
        if (millerRabin(m)) { out.push_back(m); return; }
        unsigned long long d = pollardRho(m);
        rec(d); rec(m / d);
    };
    rec(n);
    sort(out.begin(), out.end());
    return out;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ factorization.hpp -o demo && ./demo
int main() {
    printf("millerRabin(1000000007)=%d ; 1000000007*1000000009 nguyên tố? %d\n",
           (int)millerRabin(1000000007ull), (int)millerRabin(1000000007ull * 1000000009ull));   // 1 ; 0
    printf("factorize(360) = ");
    for (auto p : factorize(360)) printf("%llu ", p);   // 2 2 2 3 3 5
    printf("\n");
    return 0;
}
#endif
