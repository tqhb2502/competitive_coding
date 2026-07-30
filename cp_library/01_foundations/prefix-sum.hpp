#pragma once
#include <bits/stdc++.h>
using namespace std;
// Prefix Sum — tổng đoạn trên mảng tĩnh (không cập nhật).
// Khi dùng: nhiều truy vấn tổng đoạn [l, r), mảng không đổi.
// ĐPT: build O(n); query O(1); bộ nhớ O(n).
// Dùng: PrefixSum ps(a); ps.sum(l, r); // tổng a[l..r-1] (0-based, nửa mở [l, r))
// Bẫy: dùng long long tránh tràn; r là biên phải MỞ; sum(0, n) = tổng cả mảng.
// CSES: 1148 1189 1203 1644 1646 1652 1661 1662 1736 2081 2086 2087 2088 2110 2115 2186 2416 3190 3294 3301 3361 3411 3418
struct PrefixSum {
    vector<long long> pre;
    // Ý nghĩa: dựng mảng tiền tố pre với pre[0]=0, pre[i+1]=pre[i]+a[i] (ép sang long long).
    // Tham số: a = mảng đầu vào (0-based) kiểu bất kỳ chuyển được sang long long.
    template <class T>
    explicit PrefixSum(const vector<T>& a) : pre(a.size() + 1, 0) {
        for (size_t i = 0; i < a.size(); i++) pre[i + 1] = pre[i] + (long long)a[i];
    }
    // Ý nghĩa: tổng đoạn nửa mở [l, r) tức a[l..r-1] (0-based).
    // Tham số: l = biên trái đóng; r = biên phải mở.
    // Trả về: pre[r] - pre[l], tổng các phần tử từ chỉ số l đến r-1.
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
