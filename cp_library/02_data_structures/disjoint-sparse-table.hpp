#pragma once
#include <bits/stdc++.h>
using namespace std;
// Disjoint Sparse Table — truy vấn phép KẾT HỢP bất kỳ (không cần nghịch đảo) O(1).
// Khi dùng: range gcd / product-mod / min-count... trên mảng tĩnh, cần query O(1) cho op không nghịch đảo.
// ĐPT: build O(n log n); query O(1); bộ nhớ O(n log n). Đoạn nửa mở [l, r), l < r.
// Dùng: DisjointSparseTable st(a, [](ll x, ll y){ return __gcd(x, y); }); st.query(l, r);
// Bẫy: op phải KẾT HỢP; query kết hợp đúng thứ tự trái->phải nên non-commutative cũng đúng.
struct DisjointSparseTable {
    int n;
    vector<vector<long long>> t;
    vector<int> lg;
    function<long long(long long, long long)> op;
    DisjointSparseTable(const vector<long long>& a,
                        function<long long(long long, long long)> combine)
        : n((int)a.size()), op(move(combine)) {
        int LGN = 1;
        while ((1 << LGN) < n) ++LGN;
        int maxv = 1 << LGN;  // lớn hơn mọi l^r với l, r trong [0, n)
        lg.assign(maxv + 1, 0);
        for (int i = 2; i <= maxv; i++) lg[i] = lg[i / 2] + 1;
        int levels = LGN;
        t.assign(levels, vector<long long>(max(1, n)));
        for (int level = 0; level < levels; level++) {
            int block = 1 << (level + 1);
            for (int start = 0; start < n; start += block) {
                int mid = min(n, start + (block >> 1));
                int end = min(n, start + block);
                if (mid - 1 >= start) {  // nửa trái: gộp hậu tố từ mid-1 về start
                    t[level][mid - 1] = a[mid - 1];
                    for (int i = mid - 2; i >= start; i--) t[level][i] = op(a[i], t[level][i + 1]);
                }
                if (mid < end) {  // nửa phải: gộp tiền tố từ mid tới end-1
                    t[level][mid] = a[mid];
                    for (int i = mid + 1; i < end; i++) t[level][i] = op(t[level][i - 1], a[i]);
                }
            }
        }
    }
    long long query(int l, int r) const {  // [l, r) -> nội bộ dùng bao đóng [l, r-1]
        --r;
        if (l == r) return t[0][l];  // t[0][i] luôn = a[i]
        int level = lg[l ^ r];
        return op(t[level][l], t[level][r]);
    }
};
