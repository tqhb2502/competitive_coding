#pragma once
#include <bits/stdc++.h>
using namespace std;
// Lazy Segment Tree — cập nhật ĐOẠN + truy vấn ĐOẠN, O(log n). Bản mẫu: range-add + range-sum.
// Khi dùng: vừa "+v trên đoạn" vừa hỏi "tổng đoạn", online.
// ĐPT: build O(n); update/query O(log n); bộ nhớ O(4n). Index 0-based, đoạn [l, r).
// Dùng: LazySeg st(a); st.update(l, r, v); // +v trên [l, r); st.query(l, r); // tổng [l, r)
// Bẫy: dùng long long; đây là add/sum — đổi phép khác phải sửa apply()/gộp; n phải >= 1.
// CSES: 1735 1741 2425
struct LazySeg {
    int n;
    vector<long long> sum, lz;
    explicit LazySeg(const vector<long long>& a)
        : n((int)a.size()), sum(4 * max(1, n), 0), lz(4 * max(1, n), 0) {
        if (n > 0) build(1, 0, n, a);
    }
    void build(int node, int l, int r, const vector<long long>& a) {
        if (r - l == 1) { sum[node] = a[l]; return; }
        int m = (l + r) / 2;
        build(2 * node, l, m, a);
        build(2 * node + 1, m, r, a);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }
    void apply(int node, int l, int r, long long v) {
        sum[node] += (long long)(r - l) * v;
        lz[node] += v;
    }
    void push(int node, int l, int r) {
        if (lz[node]) {
            int m = (l + r) / 2;
            apply(2 * node, l, m, lz[node]);
            apply(2 * node + 1, m, r, lz[node]);
            lz[node] = 0;
        }
    }
    void update(int node, int l, int r, int ql, int qr, long long v) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) { apply(node, l, r, v); return; }
        push(node, l, r);
        int m = (l + r) / 2;
        update(2 * node, l, m, ql, qr, v);
        update(2 * node + 1, m, r, ql, qr, v);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }
    long long query(int node, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) return sum[node];
        push(node, l, r);
        int m = (l + r) / 2;
        return query(2 * node, l, m, ql, qr) + query(2 * node + 1, m, r, ql, qr);
    }
    void update(int l, int r, long long v) { update(1, 0, n, l, r, v); }  // [l, r)
    long long query(int l, int r) { return query(1, 0, n, l, r); }        // [l, r)
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ lazy-segment-tree.hpp -o demo && ./demo
int main() {
    vector<long long> a = {1, 2, 3, 4, 5};
    LazySeg st(a);
    st.update(1, 4, 10);   // +10 tren [1,4)
    printf("sum[0,5)=%lld  sum[1,4)=%lld\n", st.query(0, 5), st.query(1, 4));
    return 0;
}
#endif
