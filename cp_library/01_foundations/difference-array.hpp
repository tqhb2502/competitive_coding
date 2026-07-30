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
    // Ý nghĩa: khởi tạo mảng hiệu d gồm n+1 phần tử 0 để chuẩn bị cộng đoạn.
    // Tham số: n = số phần tử của mảng kết quả (index 0-based).
    explicit DiffArray(int n) : d(n + 1, 0) {}
    // Ý nghĩa: ghi nhận lệnh cộng v cho mọi vị trí trong đoạn nửa mở [l, r) (chỉ đánh dấu, O(1)).
    // Tham số: l = đầu đoạn (gồm); r = cuối đoạn (không gồm); v = giá trị cộng thêm.
    void add(int l, int r, long long v) { d[l] += v; d[r] -= v; }  // [l, r)
    // Ý nghĩa: lấy tổng tiền tố của mảng hiệu để dựng mảng kết quả cuối cùng.
    // Trả về: vector độ dài n với a[i] = tổng các delta phủ vị trí i (0-based).
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
