#pragma once
#include <bits/stdc++.h>
using namespace std;
// Prefix Sum — tổng đoạn trên mảng tĩnh (không cập nhật).
// Khi dùng: nhiều truy vấn tổng đoạn [l, r), mảng không đổi.
// ĐPT: build O(n); query O(1); bộ nhớ O(n).
// Dùng: PrefixSum ps(a); ps.sum(l, r); // tổng a[l..r-1] (0-based, nửa mở [l, r))
// Bẫy: dùng long long tránh tràn; r là biên phải MỞ; sum(0, n) = tổng cả mảng.
// CSES: 1148 1189 1203 1644 1646 1652 1661 1662 1736 2081 2086 2087 2088 2110 2115 2186 2416 3190 3301 3361 3411 3418
struct PrefixSum {
    vector<long long> pre;
    template <class T>
    explicit PrefixSum(const vector<T>& a) : pre(a.size() + 1, 0) {
        for (size_t i = 0; i < a.size(); i++) pre[i + 1] = pre[i] + (long long)a[i];
    }
    long long sum(int l, int r) const { return pre[r] - pre[l]; }  // [l, r)
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ prefix-sum.hpp -o demo && ./demo
int main() {
    vector<int> a = {3, 1, 4, 1, 5};
    PrefixSum ps(a);
    printf("sum[1,4)=%lld  sum[0,5)=%lld\n", ps.sum(1, 4), ps.sum(0, 5));
    return 0;
}
#endif
