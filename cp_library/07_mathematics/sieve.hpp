#pragma once
#include <bits/stdc++.h>
using namespace std;
// Linear Sieve — sàng tuyến tính O(n): ước nguyên tố NHỎ NHẤT (spf), danh sách nguyên tố, phân tích thừa số nhanh.
// Khi dùng: kiểm tra nguyên tố, phân tích thừa số, đếm/tổng ước, các hàm số học nhân tính tới n.
// ĐPT: dựng O(n); factorize O(log x); bộ nhớ O(n). spf[x] = ước nguyên tố nhỏ nhất của x (x >= 2).
// Dùng: Sieve S(n); S.isPrime(x); S.factorize(x); // trả các cặp (nguyên tố, số mũ)
// Bẫy: chỉ dùng cho x <= n đã sàng; 0 và 1 không nguyên tố; factorize dùng spf nên rất nhanh.
// CSES: 1081 1713 3398 3423
struct Sieve {
    vector<int> spf, primes;
    // Ý nghĩa: sàng tuyến tính tính ước nguyên tố nhỏ nhất spf[i] và danh sách nguyên tố tới n.
    // Tham số: n = giới hạn trên của miền sàng.
    explicit Sieve(int n) : spf(n + 1, 0) {
        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) { spf[i] = i; primes.push_back(i); }
            for (int p : primes) {
                if (p > spf[i] || (long long)i * p > n) break;
                spf[(long long)i * p] = p;
            }
        }
    }
    // Ý nghĩa: kiểm tra x có phải số nguyên tố (trong miền đã sàng).
    // Tham số: x = số cần kiểm tra.
    // Trả về: true nếu x >= 2 và x là số nguyên tố.
    bool isPrime(int x) const { return x >= 2 && spf[x] == x; }
    // Ý nghĩa: phân tích x thành thừa số nguyên tố bằng spf.
    // Tham số: x = số cần phân tích (2 <= x <= n).
    // Trả về: danh sách cặp (nguyên tố, số mũ) theo thứ tự tăng dần của nguyên tố.
    vector<pair<int, int>> factorize(int x) const {
        vector<pair<int, int>> res;
        while (x > 1) {
            int p = spf[x], c = 0;
            while (x % p == 0) { x /= p; c++; }
            res.push_back({p, c});
        }
        return res;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ sieve.hpp -o demo && ./demo
int main() {
    Sieve S(100);
    printf("isPrime(17)=%d ; số nguyên tố <=100 = %d ; 60 = ", (int)S.isPrime(17), (int)S.primes.size());   // 1 ; 25
    for (auto [p, c] : S.factorize(60)) printf("%d^%d ", p, c);   // 2^2 3^1 5^1
    printf("\n");
    return 0;
}
#endif
