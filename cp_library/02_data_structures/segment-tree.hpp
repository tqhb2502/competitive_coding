#pragma once
#include <bits/stdc++.h>
using namespace std;
// Segment Tree (iterative) — cập nhật ĐIỂM, truy vấn ĐOẠN theo monoid, O(log n).
// Khi dùng: update điểm xen truy vấn đoạn với phép KẾT HỢP (sum/min/max/gcd...).
// ĐPT: build O(n); update/query O(log n); bộ nhớ O(2n). Index 0-based, đoạn [l, r).
// Dùng: SegTree st(a, [](ll x,ll y){return min(x,y);}, LLONG_MAX); st.set(i, v); st.query(l, r);
// Bẫy: id là phần tử trung hòa của op; bản lặp này gộp đúng thứ tự trái->phải (non-commutative cũng đúng).
struct SegTree {
    int n;
    vector<long long> t;
    function<long long(long long, long long)> op;
    long long id;
    SegTree(const vector<long long>& a, function<long long(long long, long long)> combine, long long identity)
        : n((int)a.size()), t(2 * max(1, (int)a.size()), identity), op(move(combine)), id(identity) {
        for (int i = 0; i < n; i++) t[n + i] = a[i];
        for (int i = n - 1; i >= 1; i--) t[i] = op(t[2 * i], t[2 * i + 1]);
    }
    void set(int i, long long v) {  // gán a[i] = v
        for (t[i += n] = v; i > 1; i >>= 1) t[i >> 1] = op(t[i], t[i ^ 1]);
    }
    long long query(int l, int r) const {  // [l, r)
        long long resl = id, resr = id;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = op(resl, t[l++]);
            if (r & 1) resr = op(t[--r], resr);
        }
        return op(resl, resr);
    }
};
