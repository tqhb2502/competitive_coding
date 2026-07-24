#pragma once
#include <bits/stdc++.h>
using namespace std;
// Difference Array — cộng đoạn nhiều lần (offline) rồi lấy mảng kết quả.
// Khi dùng: nhiều lệnh "+v trên [l, r)", chỉ cần đọc mảng sau khi xong.
// ĐPT: add O(1); build O(n); bộ nhớ O(n).
// Dùng: DiffArray d(n); d.add(l, r, v); auto res = d.build(); // res[i] = tổng delta phủ i
// Bẫy: đoạn nửa mở [l, r); dùng long long; add hết rồi mới build một lần.
// CSES: 1138 1148 1203 1651 1736 2110 2416
struct DiffArray {
    vector<long long> d;
    explicit DiffArray(int n) : d(n + 1, 0) {}
    void add(int l, int r, long long v) { d[l] += v; d[r] -= v; }  // [l, r)
    vector<long long> build() const {
        int n = (int)d.size() - 1;
        vector<long long> a(n);
        long long cur = 0;
        for (int i = 0; i < n; i++) { cur += d[i]; a[i] = cur; }
        return a;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ difference-array.hpp -o demo && ./demo
int main() {
    DiffArray d(5);
    d.add(1, 4, 10);   // +10 tren [1,4)
    d.add(0, 2, 5);    // +5  tren [0,2)
    for (long long x : d.build()) printf("%lld ", x);
    printf("\n");
    return 0;
}
#endif
