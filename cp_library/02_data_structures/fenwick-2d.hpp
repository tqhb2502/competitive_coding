#pragma once
#include <bits/stdc++.h>
using namespace std;
// Fenwick Tree 2D — cập nhật ĐIỂM, tổng HÌNH CHỮ NHẬT, O(log R · log C).
// Khi dùng: lưới kích thước cố định (R·C vừa phải), update ô + tổng vùng con.
// ĐPT: add/query O(log R · log C); bộ nhớ O(R·C). Ngoài 0-based; trong 1-based.
// Dùng: Fenwick2D f(R, C); f.add(r, c, v); f.rectSum(r1, c1, r2, c2); // [r1,r2) × [c1,c2)
// Bẫy: tốn O(R·C) bộ nhớ — tọa độ lớn phải nén trước; dùng long long.
// CSES: 1739
struct Fenwick2D {
    int R, C;
    vector<vector<long long>> t;
    // Ý nghĩa: khởi tạo Fenwick 2D kích thước R×C, mọi ô ban đầu bằng 0.
    // Tham số: R = số hàng; C = số cột (tọa độ ngoài 0-based).
    Fenwick2D(int R, int C) : R(R), C(C), t(R + 1, vector<long long>(C + 1, 0)) {}
    // Ý nghĩa: cộng thêm v vào ô (r, c) (cập nhật điểm, tọa độ 0-based).
    // Tham số: r = hàng; c = cột; v = lượng cộng thêm.
    void add(int r, int c, long long v) {
        for (int i = r + 1; i <= R; i += i & -i)
            for (int j = c + 1; j <= C; j += j & -j) t[i][j] += v;
    }
    // Ý nghĩa: tổng tiền tố vùng [0, r) × [0, c) (nửa mở, tọa độ 0-based).
    // Tham số: r = biên hàng (loại trừ); c = biên cột (loại trừ).
    // Trả về: tổng các ô trong hình chữ nhật từ gốc tới (r, c).
    long long sum(int r, int c) const {  // [0, r) × [0, c)
        long long s = 0;
        for (int i = r; i > 0; i -= i & -i)
            for (int j = c; j > 0; j -= j & -j) s += t[i][j];
        return s;
    }
    // Ý nghĩa: tổng vùng hình chữ nhật [r1, r2) × [c1, c2) qua bù trừ 4 tổng tiền tố.
    // Tham số: r1, r2 = biên hàng (nửa mở); c1, c2 = biên cột (nửa mở); tọa độ 0-based.
    // Trả về: tổng các ô trong hình chữ nhật con.
    long long rectSum(int r1, int c1, int r2, int c2) const {  // [r1, r2) × [c1, c2)
        return sum(r2, c2) - sum(r1, c2) - sum(r2, c1) + sum(r1, c1);
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ fenwick-2d.hpp -o demo && ./demo
int main() {
    Fenwick2D f(3, 3);
    f.add(0, 0, 5);
    f.add(1, 2, 7);
    printf("rectSum [0,2)x[0,3) = %lld\n", f.rectSum(0, 0, 2, 3));   // 12
    return 0;
}
#endif
