#pragma once
#include <bits/stdc++.h>
using namespace std;
// Dynamic Segment Tree — cây phân đoạn trên miền [0, N) RẤT LỚN, tạo node khi cần, O(log N).
// Khi dùng: chỉ số/tọa độ tới ~1e9 không muốn nén; point update + range sum online.
// ĐPT: update/query O(log N); bộ nhớ O(số update · log N). N là kích thước MIỀN, không phải số phần tử.
// Dùng: DynSeg st(N); st.add(pos, v); st.sum(l, r); // cộng v vào a[pos]; tổng [l, r) nửa mở
// Bẫy: dùng long long; pos trong [0, N); node cấp phát động (không realloc-cache tham chiếu Node&).
// CSES: 1144
struct DynSeg {
    struct Node { long long sum = 0; int l = -1, r = -1; };
    vector<Node> t;
    long long N;
    // Ý nghĩa: khởi tạo cây rỗng trên miền [0, N), tạo sẵn node gốc (index 0).
    // Tham số: N = kích thước miền chỉ số (không phải số phần tử).
    explicit DynSeg(long long N) : N(N) {
        if (N <= 0) throw invalid_argument("DynSeg: N phải dương");
        t.push_back(Node());
    }  // node 0 = gốc
    // Ý nghĩa: cộng v vào vị trí pos trong đoạn [lo, hi) của node, cấp phát con động khi cần.
    // Tham số: node = index node hiện tại; lo, hi = biên nửa mở [lo, hi) node quản; pos = vị trí cập nhật; v = lượng cộng.
    void addRec(int node, long long lo, long long hi, long long pos, long long v) {
        t[node].sum += v;
        if (hi - lo == 1) return;
        long long mid = lo + (hi - lo) / 2;
        if (pos < mid) {
            if (t[node].l == -1) { int c = (int)t.size(); t.push_back(Node()); t[node].l = c; }
            addRec(t[node].l, lo, mid, pos, v);
        } else {
            if (t[node].r == -1) { int c = (int)t.size(); t.push_back(Node()); t[node].r = c; }
            addRec(t[node].r, mid, hi, pos, v);
        }
    }
    // Ý nghĩa: tính tổng phần giao giữa đoạn node [lo, hi) và khoảng truy vấn [ql, qr).
    // Tham số: node = index node (-1 nếu chưa tạo); lo, hi = biên nửa mở node quản; ql, qr = khoảng truy vấn [ql, qr).
    // Trả về: tổng các giá trị trong phần giao, 0 nếu node trống hoặc không giao.
    long long sumRec(int node, long long lo, long long hi, long long ql, long long qr) const {
        if (node == -1 || qr <= lo || hi <= ql) return 0;
        if (ql <= lo && hi <= qr) return t[node].sum;
        long long mid = lo + (hi - lo) / 2;
        return sumRec(t[node].l, lo, mid, ql, qr) + sumRec(t[node].r, mid, hi, ql, qr);
    }
    // Ý nghĩa: cộng v vào a[pos] (điểm cập nhật) trên toàn miền [0, N).
    // Tham số: pos = vị trí cập nhật trong [0, N); v = lượng cộng.
    void add(long long pos, long long v) {
        if (pos < 0 || pos >= N) throw out_of_range("DynSeg::add");
        addRec(0, 0, N, pos, v);
    }
    // Ý nghĩa: tính tổng các phần tử trên khoảng nửa mở [l, r).
    // Tham số: l, r = biên nửa mở [l, r) của truy vấn.
    // Trả về: tổng a[l] + ... + a[r-1].
    long long sum(long long l, long long r) const {
        if (l < 0 || l > r || r > N) throw out_of_range("DynSeg::sum");
        return sumRec(0, 0, N, l, r);
    }  // [l, r)
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ dynamic-segment-tree.hpp -o demo && ./demo
int main() {
    DynSeg st(1000000000LL);
    st.add(5, 3);
    st.add(999999999LL, 7);
    printf("sum[0,1e9)=%lld  sum[6,1e9)=%lld\n",
           st.sum(0, 1000000000LL), st.sum(6, 1000000000LL));   // 10  7
    return 0;
}
#endif
