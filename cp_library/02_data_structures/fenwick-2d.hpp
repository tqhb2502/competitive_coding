#pragma once
#include <bits/stdc++.h>
using namespace std;
// Fenwick Tree 2D — cập nhật ĐIỂM, tổng HÌNH CHỮ NHẬT, O(log R · log C).
// Khi dùng: lưới kích thước cố định (R·C vừa phải), update ô + tổng vùng con.
// ĐPT: add/query O(log R · log C); bộ nhớ O(R·C). Ngoài 0-based; trong 1-based.
// Dùng: Fenwick2D f(R, C); f.add(r, c, v); f.rectSum(r1, c1, r2, c2); // [r1,r2) × [c1,c2)
// Bẫy: tốn O(R·C) bộ nhớ — tọa độ lớn phải nén trước; dùng long long.
struct Fenwick2D {
    int R, C;
    vector<vector<long long>> t;
    Fenwick2D(int R, int C) : R(R), C(C), t(R + 1, vector<long long>(C + 1, 0)) {}
    void add(int r, int c, long long v) {
        for (int i = r + 1; i <= R; i += i & -i)
            for (int j = c + 1; j <= C; j += j & -j) t[i][j] += v;
    }
    long long sum(int r, int c) const {  // [0, r) × [0, c)
        long long s = 0;
        for (int i = r; i > 0; i -= i & -i)
            for (int j = c; j > 0; j -= j & -j) s += t[i][j];
        return s;
    }
    long long rectSum(int r1, int c1, int r2, int c2) const {  // [r1, r2) × [c1, c2)
        return sum(r2, c2) - sum(r1, c2) - sum(r2, c1) + sum(r1, c1);
    }
};
