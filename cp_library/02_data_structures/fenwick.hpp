#pragma once
#include <bits/stdc++.h>
using namespace std;
// Fenwick Tree (BIT) — cập nhật ĐIỂM, tổng tiền tố / đoạn, O(log n).
// Khi dùng: nhiều update điểm xen truy vấn tổng đoạn (phép có nghịch đảo: cộng).
// ĐPT: add/sum O(log n); bộ nhớ O(n). Ngoài 0-based; cây bên trong 1-based (tự +1).
// Dùng: Fenwick f(n); f.add(i, v); f.sum(r); // [0, r); f.rangeSum(l, r); // [l, r)
// Bẫy: dùng long long tránh tràn; r/l là biên nửa mở; range-update xem ghi chú cuối file.
// CSES: 1137 1138 1144 1162 1648 1651 1734 1736 1740 1747 1749 2169 2420 3223
struct Fenwick {
    int n;
    vector<long long> t;
    explicit Fenwick(int n) : n(n), t(n + 1, 0) {}
    void add(int i, long long v) { for (++i; i <= n; i += i & -i) t[i] += v; }        // a[i] += v
    long long sum(int r) const { long long s = 0; for (; r > 0; r -= r & -r) s += t[r]; return s; }  // [0, r)
    long long rangeSum(int l, int r) const { return sum(r) - sum(l); }               // [l, r)
};
// Range-update + point-query : nuôi Fenwick trên mảng hiệu — add(l,+v), add(r,-v); giá trị a[i] = sum(i+1).
// Range-update + range-query : cần hai Fenwick (hoặc dùng lazy-segment-tree cho tổng quát).

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ fenwick.hpp -o demo && ./demo
int main() {
    Fenwick f(5);
    f.add(0, 3);
    f.add(2, 5);
    printf("sum[0,3)=%lld  rangeSum(1,5)=%lld\n", f.sum(3), f.rangeSum(1, 5));
    return 0;
}
#endif
