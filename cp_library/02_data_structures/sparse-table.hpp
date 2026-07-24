#pragma once
#include <bits/stdc++.h>
using namespace std;
// Sparse Table — truy vấn IDEMPOTENT (min/max/gcd) trên mảng TĨNH, query O(1).
// Khi dùng: nhiều truy vấn min/max/gcd đoạn, mảng không đổi.
// ĐPT: build O(n log n); query O(1); bộ nhớ O(n log n). Đoạn nửa mở [l, r), l < r.
// Dùng: SparseTable st(a); st.query(l, r); // min trên [l, r)  (đổi min -> max/gcd nếu cần)
// Bẫy: CHỈ dùng cho phép idempotent (đoạn chồng lấn vẫn đúng); tổng thì KHÔNG được.
struct SparseTable {
    vector<vector<long long>> t;
    vector<int> lg;
    explicit SparseTable(const vector<long long>& a) {
        int n = (int)a.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;
        int K = (n ? lg[n] : 0) + 1;
        t.assign(K, vector<long long>(max(1, n)));
        for (int i = 0; i < n; i++) t[0][i] = a[i];
        for (int k = 1; k < K; k++)
            for (int i = 0; i + (1 << k) <= n; i++)
                t[k][i] = min(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
    }
    long long query(int l, int r) const {  // min trên [l, r), l < r
        int k = lg[r - l];
        return min(t[k][l], t[k][r - (1 << k)]);
    }
};
