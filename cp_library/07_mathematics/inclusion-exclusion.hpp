#pragma once
#include <bits/stdc++.h>
using namespace std;
// Inclusion-Exclusion — mẫu bao hàm–loại trừ: đếm số chia hết bởi ÍT NHẤT MỘT phần tử, và số hoán vị không điểm cố định.
// Khi dùng: "đếm x trong [1,n] chia hết bởi ít nhất một trong k số", "đếm hoán vị mà không ai giữ vị trí của mình".
// ĐPT: countDivisibleByAny O(2^k * k) (k <= ~20, tính cả gộp lcm); derangements O(n); bộ nhớ O(1).
// Dùng: countDivisibleByAny(10, {2, 3, 5});        // 8
//       derangements(4, 1000000007LL);             // 9
// Bẫy: mọi phần tử của primes phải >= 1 (không âm, không 0); đoạn đếm là [1, n] ĐÓNG hai đầu nên n = 0 cho 0.
//      Hàm dùng LCM chứ không dùng tích, nên tập ĐÔI MỘT NGUYÊN TỐ CÙNG NHAU hay không đều đúng (trùng lặp cũng đúng);
//      nếu tự viết bằng tích các phần tử thì BẮT BUỘC phải đôi một nguyên tố cùng nhau. lcm gộp bằng __int128 để tránh tràn.
//      derangements: D(0) = 1 (hoán vị rỗng), D(1) = 0; mod phải >= 1 (mod = 1 thì mọi kết quả là 0); nhân qua __int128.
// CSES: 1075 1717 2185 2228 2421 2429 3163 3215 3400
// Ý nghĩa: đếm số nguyên x trong [1, n] chia hết bởi ít nhất một phần tử của primes (bao hàm–loại trừ trên 2^k mask).
// Tham số: n = giới hạn trên của đoạn [1, n] (n >= 0); primes = tập số chia, mỗi phần tử >= 1, kích thước k <= ~20.
// Trả về: số lượng x thỏa mãn (luôn <= n, vừa long long).
long long countDivisibleByAny(long long n, const vector<long long>& primes) {
    int k = (int)primes.size();
    if (n <= 0 || k == 0) return 0;
    long long res = 0;
    int total = 1 << k;
    for (int mask = 1; mask < total; mask++) {
        long long lcmVal = 1;
        int bits = 0;
        bool tooBig = false;
        for (int i = 0; i < k; i++) {
            if (!((mask >> i) & 1)) continue;
            long long p = primes[i];
            if (p <= 0) { tooBig = true; break; }   // chốt: phần tử <= 0 sẽ gây chia cho 0, bỏ mọi mask chứa nó
            bits++;
            long long part = lcmVal / std::gcd(lcmVal, p);
            // Nếu lcm đã vượt n thì không còn số nào trong [1, n] chia hết cho nó -> bỏ mask này.
            if ((__int128)part * p > (__int128)n) { tooBig = true; break; }
            lcmVal = part * p;
        }
        if (tooBig) continue;
        long long cnt = n / lcmVal;
        res += (bits & 1) ? cnt : -cnt;
    }
    return res;
}
// Ý nghĩa: đếm số hoán vị của n phần tử không có điểm cố định (derangement) theo truy hồi D(n) = (n-1) * (D(n-1) + D(n-2)).
// Tham số: n = số phần tử (n >= 0); mod = modulo (>= 1, có thể lớn hơn 3e9 vì nhân bằng __int128).
// Trả về: D(n) % mod, với D(0) = 1, D(1) = 0.
long long derangements(int n, long long mod) {
    if (n < 0) return 0;
    long long prev2 = 1 % mod;              // D(0)
    if (n == 0) return prev2;
    long long prev1 = 0;                    // D(1)
    for (int i = 2; i <= n; i++) {
        long long cur = (long long)((__int128)(i - 1) * ((prev1 + prev2) % mod) % mod);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ inclusion-exclusion.hpp -o demo && ./demo
int main() {
    printf("chia hết bởi {2,3,5} trong [1,10] = %lld\n", countDivisibleByAny(10, {2, 3, 5}));   // 8
    printf("chia hết bởi {4,6} trong [1,24]   = %lld\n", countDivisibleByAny(24, {4, 6}));      // 8 (lcm = 12)
    printf("D(1..5) =");
    for (int i = 1; i <= 5; i++) printf(" %lld", derangements(i, 1000000007LL));                 // 0 1 2 9 44
    printf("\n");
    return 0;
}
#endif
