#pragma once
#include <bits/stdc++.h>
using namespace std;
// Eulerian Numbers — A(n,k), số hoán vị n phần tử có đúng k descent, bằng recurrence O(n^2) hoặc công thức tường minh.
// Khi dùng: đếm phân phối số descent/ascents/runs; số lượt Collecting Numbers = 1 + descent của hoán vị nghịch đảo.
// ĐPT: eulerianNumberRow O(n^2) thời gian, O(n) bộ nhớ; eulerianNumberExplicitPrimeMod O((k+1) log n) thời gian, O(k) bộ nhớ.
// Dùng: auto row = eulerianNumberRow(n); row[k]; eulerianNumberExplicitPrimeMod(n,k,MOD);
// Bẫy: k là số descent 0-based (CSES hỏi lượt r thì dùng A(n,r-1)); A(0,0)=1, còn với n>0 chỉ k in [0,n-1].
//       API explicit yêu cầu mod NGUYÊN TỐ và k+1 < mod; không kiểm tra tính nguyên tố. Mọi phép nhân mod dùng __int128.
// CSES: 3157

// Ý nghĩa: tính cả hàng Eulerian A(n,0..n-1) bằng A(m,k)=(k+1)A(m-1,k)+(m-k)A(m-1,k-1).
// Tham số: n = kích thước hoán vị không âm; mod = modulo dương (không cần nguyên tố).
// Trả về: {1} nếu n=0; nếu n>0 trả vector dài n với phần tử k là A(n,k) modulo mod.
inline vector<long long> eulerianNumberRow(int n, long long mod = 1000000007LL) {
    if (n < 0) throw invalid_argument("eulerianNumberRow: n âm");
    if (mod <= 0) throw invalid_argument("eulerianNumberRow: mod phải dương");
    vector<long long> row(n + 1, 0);
    row[0] = 1 % mod;  // A(0,0)
    for (int length = 1; length <= n; ++length) {
        for (int descents = length - 1; descents >= 0; --descents) {
            __int128 keep = (__int128)(descents + 1) * row[descents];
            __int128 create = descents == 0
                ? 0
                : (__int128)(length - descents) * row[descents - 1];
            row[descents] = (long long)((keep + create) % mod);
        }
    }
    if (n == 0) return {row[0]};
    row.resize(n);
    return row;
}

// Ý nghĩa: tính một A(n,k) từ công thức sum_{j=0}^{k+1} (-1)^j C(n+1,j)(k+1-j)^n modulo số nguyên tố.
// Tham số: n = kích thước không âm; descents = k; primeMod = modulo nguyên tố > k+1.
// Trả về: A(n,k) modulo primeMod; trả 0 khi k ngoài miền, với quy ước A(0,0)=1.
inline long long eulerianNumberExplicitPrimeMod(
    int n, int descents, long long primeMod = 1000000007LL) {
    if (n < 0) throw invalid_argument("eulerianNumberExplicitPrimeMod: n âm");
    if (primeMod <= 1) throw invalid_argument("eulerianNumberExplicitPrimeMod: mod phải > 1");
    if (n == 0) return descents == 0 ? 1 % primeMod : 0;
    if (descents < 0 || descents >= n) return 0;
    if ((long long)descents + 1 >= primeMod)
        throw invalid_argument("eulerianNumberExplicitPrimeMod: cần k+1 < mod nguyên tố");

    auto powerMod = [&](long long base, int exponent) {
        long long result = 1 % primeMod;
        base %= primeMod;
        while (exponent > 0) {
            if (exponent & 1) result = (long long)((__int128)result * base % primeMod);
            base = (long long)((__int128)base * base % primeMod);
            exponent >>= 1;
        }
        return result;
    };

    // inv[i] theo recurrence tuyến tính, hợp lệ vì primeMod nguyên tố và 1 <= i <= k+1 < primeMod.
    vector<long long> inverse(descents + 2, 0);
    inverse[1] = 1;
    for (int i = 2; i <= descents + 1; ++i) {
        inverse[i] = (long long)((primeMod -
            (__int128)(primeMod / i) * inverse[(int)(primeMod % i)] % primeMod) % primeMod);
    }

    long long answer = 0;
    long long choose = 1;  // C(n+1,0)
    for (int j = 0; j <= descents + 1; ++j) {
        long long base = descents + 1LL - j;
        long long term = (long long)((__int128)choose * powerMod(base, n) % primeMod);
        long long signedTerm = (j & 1) ? (primeMod - term) % primeMod : term;
        answer = (long long)(((__int128)answer + signedTerm) % primeMod);

        if (j < descents + 1) {
            long long numerator = (n + 1LL - j) % primeMod;
            choose = (long long)((__int128)choose * numerator % primeMod);
            choose = (long long)((__int128)choose * inverse[j + 1] % primeMod);
        }
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ eulerian-numbers.hpp -o demo && ./demo
int main() {
    auto row = eulerianNumberRow(4);  // 1, 11, 11, 1
    printf("A(4,k):");
    for (long long value : row) printf(" %lld", value);
    printf("; explicit A(4,2)=%lld\n", eulerianNumberExplicitPrimeMod(4, 2));
    // A(4,k): 1 11 11 1; explicit A(4,2)=11
    return 0;
}
#endif
