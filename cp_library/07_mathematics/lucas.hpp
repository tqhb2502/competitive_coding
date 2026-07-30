#pragma once
#include <bits/stdc++.h>
using namespace std;
// Lucas' Theorem — C(n, k) mod p với p NGUYÊN TỐ NHỎ và n, k khổng lồ (tới 1e18): tách theo chữ số cơ số p.
// Khi dùng: tổ hợp mod nguyên tố nhỏ (p <= n), đếm đường đi/tam giác Pascal mod 2, XOR pyramid, bài "C(n,k) chẵn/lẻ".
// ĐPT: dựng bảng giai thừa mod p O(p) (nhớ lại, chỉ dựng khi p đổi); mỗi truy vấn O(log_p n); binomOddParity O(1); bộ nhớ O(p).
// Dùng: binomLucas(1000000000000000000LL, 123456789LL, 13); binomOddParity(10, 2); // true vì C(10,2)=45 lẻ
// Bẫy: p PHẢI nguyên tố và đủ nhỏ để cấp bảng O(p) (p <= 1 trả 0); k < 0 hay k > n trả 0/false; bảng giai thừa là static nên nhóm truy vấn cùng p, đổi p liên tục sẽ dựng lại mỗi lần.
// CSES: 2419 3194 3195
// Ý nghĩa: tính C(n, k) mod p theo định lý Lucas — C(n,k) ≡ ∏ C(n_i, k_i) (mod p) với n_i, k_i là các chữ số cơ số p.
// Tham số: n, k = đối số tổ hợp (có thể tới 1e18); p = modulo NGUYÊN TỐ nhỏ (dựng bảng giai thừa cỡ p).
// Trả về: C(n, k) mod p; trả 0 nếu n < 0, k < 0, k > n hoặc p <= 1.
long long binomLucas(long long n, long long k, long long p) {
    if (p <= 1 || n < 0 || k < 0 || k > n) return 0;
    // Nhân theo mod dùng __int128 để p tới ~9e18 vẫn không tràn (dù thực tế p phải nhỏ vì bảng cỡ p).
    auto mul = [&](long long a, long long b) { return (long long)((__int128)a * b % p); };
    auto mpow = [&](long long b, long long e) {
        long long r = 1 % p;
        b %= p;
        while (e) { if (e & 1) r = mul(r, b); b = mul(b, b); e >>= 1; }
        return r;
    };
    // Bảng giai thừa + nghịch đảo giai thừa mod p, nhớ lại giữa các lần gọi cùng p.
    static long long cachedP = 0;
    static vector<long long> fact, invFact;
    if (cachedP != p) {
        cachedP = p;
        fact.assign((size_t)p, 1);
        invFact.assign((size_t)p, 1);
        for (long long i = 1; i < p; i++) fact[(size_t)i] = mul(fact[(size_t)(i - 1)], i % p);
        // (p-1)! ≡ -1 (mod p) theo Wilson nên luôn khả nghịch; lùi dần cho invFact còn lại.
        invFact[(size_t)(p - 1)] = mpow(fact[(size_t)(p - 1)], p - 2);
        for (long long i = p - 1; i > 0; i--) invFact[(size_t)(i - 1)] = mul(invFact[(size_t)i], i % p);
    }
    long long res = 1 % p;
    while (n > 0 || k > 0) {
        long long ni = n % p, ki = k % p;
        if (ki > ni) return 0;   // một chữ số của k lớn hơn của n => C(n_i, k_i) = 0 => cả tích ≡ 0
        long long c = mul(fact[(size_t)ni], mul(invFact[(size_t)ki], invFact[(size_t)(ni - ki)]));
        res = mul(res, c);
        n /= p;
        k /= p;
    }
    return res;
}

// Ý nghĩa: kiểm tra C(n, k) là số LẺ — Lucas mod 2 cho C(n,k) lẻ <=> mọi bit của k đều nằm trong n <=> (k & ~n) == 0.
// Tham số: n, k = đối số tổ hợp (tới 1e18, không âm).
// Trả về: true nếu C(n, k) lẻ; false nếu chẵn (kể cả khi k < 0 hoặc k > n, lúc đó C = 0).
bool binomOddParity(long long n, long long k) {
    if (n < 0 || k < 0 || k > n) return false;
    return ((unsigned long long)k & ~(unsigned long long)n) == 0ULL;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ lucas.hpp -o demo && ./demo
int main() {
    printf("C(10,3)%%7 = %lld ; C(6,3)%%5 = %lld ; C(5,2)%%13 = %lld\n",
           binomLucas(10, 3, 7), binomLucas(6, 3, 5), binomLucas(5, 2, 13));   // 120%7=1 ; 20%5=0 ; 10%13=10
    // n = 1e18: k = 167291695408408896 có mọi chữ số cơ số 13 <= của n nên tích khác 0; còn k = 123456789 bị chặn ở 1 chữ số.
    printf("C(1e18, 167291695408408896) %% 13 = %lld ; C(1e18, 123456789) %% 13 = %lld\n",
           binomLucas(1000000000000000000LL, 167291695408408896LL, 13),
           binomLucas(1000000000000000000LL, 123456789LL, 13));   // 7 ; 0
    printf("le(10,2)=%d le(10,4)=%d le(1e18,1e18)=%d\n",
           (int)binomOddParity(10, 2), (int)binomOddParity(10, 4),
           (int)binomOddParity(1000000000000000000LL, 1000000000000000000LL));   // 1 ; 0 ; 1
    return 0;
}
#endif
