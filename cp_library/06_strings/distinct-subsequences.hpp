#pragma once
#include <bits/stdc++.h>
using namespace std;
// Distinct Subsequences — đếm số subsequence phân biệt bằng DP trừ đóng góp tại lần xuất hiện gần nhất.
// Khi dùng: đếm subsequence phân biệt của một xâu dài; cần loại trùng do các ký tự lặp lại.
// ĐPT: O(n + 256), bộ nhớ O(256). Hỗ trợ mọi byte trong std::string.
// Dùng: long long ways = countDistinctSubsequences(s); // không tính xâu rỗng, mặc định mod 1e9+7
// Bẫy: đây là subsequence (không cần liên tiếp), không phải substring; mod phải dương; dùng __int128 khi mod lớn.
// CSES: 1149 2430
// Ý nghĩa: đếm số subsequence phân biệt khác rỗng của s theo modulo.
// Tham số: s = xâu byte bất kỳ; mod = modulo dương.
// Trả về: số subsequence phân biệt KHÁC RỖNG trong [0, mod).
long long countDistinctSubsequences(const string& s, long long mod = 1000000007LL) {
    if (mod <= 0) throw invalid_argument("countDistinctSubsequences: mod must be positive");
    array<long long, 256> previous{};
    array<bool, 256> appeared{};
    long long ways_including_empty = 1 % mod;
    for (unsigned char ch : s) {
        long long old = ways_including_empty;
        __int128 next = (__int128)2 * old;
        if (appeared[ch]) next -= previous[ch];
        next %= mod;
        if (next < 0) next += mod;
        previous[ch] = old;
        appeared[ch] = true;
        ways_including_empty = (long long)next;
    }
    if (mod == 1) return 0;
    return ways_including_empty == 0 ? mod - 1 : ways_including_empty - 1;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ distinct-subsequences.hpp -o demo && ./demo
int main() {
    printf("distinct subsequences của 'aba' = %lld\n", countDistinctSubsequences("aba"));  // a,b,aa,ab,ba,aba = 6
    return 0;
}
#endif
