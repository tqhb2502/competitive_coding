#pragma once
#include <bits/stdc++.h>
using namespace std;
// Segment Tree (iterative) — monoid T tùy ý: cập nhật ĐIỂM, gộp ĐOẠN, tìm vị trí đầu tiên, O(log n).
// Khi dùng: update điểm + query sum/min/max/gcd/Node; hoặc tìm trái nhất nơi max/sum/count đạt ngưỡng.
// ĐPT: build O(n); set/query/findFirst O(log n); bộ nhớ O(2·pow2(n)). Index 0-based, đoạn [l, r).
// Dùng: SegTree st(a, op, id); hoặc GenericSegTree<Node, Merge> st(a, Merge{}, identity); st.findFirst(l, pred);
// Bẫy: id phải trung hòa; op kết hợp; pred của findFirst phải đơn điệu false->true khi nối thêm bên phải.
// CSES: 1143 1190 1649 1742 2133 2134 2163 2166 2184 2206 3219 3226 3356 3428 3430
template<class T, class Combine>
struct GenericSegTree {
    int n, size;
    vector<T> t;
    Combine op;
    T id;
    // Ý nghĩa: dựng segment tree monoid tổng quát; lá đệm ngoài mảng mang giá trị identity.
    // Tham số: a = mảng 0-based; combine = phép kết hợp kết hợp; identity = phần tử trung hòa hai phía.
    GenericSegTree(const vector<T>& a, Combine combine, T identity)
        : n((int)a.size()), size(1), op(std::move(combine)), id(std::move(identity)) {
        while (size < max(1, n)) size <<= 1;
        t.assign(2 * size, id);
        for (int i = 0; i < n; ++i) t[size + i] = a[i];
        for (int i = size - 1; i >= 1; --i) t[i] = op(t[2 * i], t[2 * i + 1]);
    }
    // Ý nghĩa: gán a[i] = v rồi cập nhật lại các nút tổ tiên trên đường về gốc.
    // Tham số: i = vị trí cần gán (0-based); v = giá trị mới.
    void set(int i, const T& v) {
        assert(0 <= i && i < n);
        for (t[i += size] = v; i > 1; i >>= 1) {
            int p = i >> 1;
            t[p] = op(t[2 * p], t[2 * p + 1]);  // luôn gộp con trái trước con phải
        }
    }
    // Ý nghĩa: gộp các phần tử trên đoạn nửa mở [l, r) theo op, giữ đúng thứ tự trái->phải.
    // Tham số: l, r = biên trái đóng và biên phải mở (0-based).
    // Trả về: kết quả op trên [l, r), hoặc id nếu đoạn rỗng.
    T query(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        T resl = id, resr = id;
        for (l += size, r += size; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = op(resl, t[l++]);
            if (r & 1) resr = op(t[--r], resr);
        }
        return op(resl, resr);
    }
    // Ý nghĩa: tìm index nhỏ nhất i >= l sao cho pred(op(a[l],...,a[i])) là true.
    // Tham số: l = điểm bắt đầu; pred = predicate false->true đơn điệu khi nối phần tử về bên phải.
    // Trả về: index đầu tiên thỏa, hoặc n nếu không có; nếu pred(id)=true thì trả ngay l.
    template<class Predicate>
    int findFirst(int l, Predicate pred) const {
        assert(0 <= l && l <= n);
        if (l == n) return n;
        T accumulated = id;
        if (pred(accumulated)) return l;
        int node = l + size;
        do {
            while ((node & 1) == 0) node >>= 1;
            T withNode = op(accumulated, t[node]);
            if (pred(withNode)) {
                while (node < size) {
                    node <<= 1;
                    T withLeft = op(accumulated, t[node]);
                    if (!pred(withLeft)) {
                        accumulated = std::move(withLeft);
                        ++node;
                    }
                }
                return min(node - size, n);
            }
            accumulated = std::move(withNode);
            ++node;
        } while ((node & -node) != node);
        return n;
    }
};

// Alias tương thích, tiện cho trường hợp phổ biến long long + lambda/std::function.
struct SegTree : GenericSegTree<long long, function<long long(long long, long long)>> {
    using Base = GenericSegTree<long long, function<long long(long long, long long)>>;
    // Ý nghĩa: dựng bản long long tương thích API cũ từ mảng, hàm gộp và identity.
    // Tham số: a = mảng; combine = phép gộp; identity = phần tử trung hòa.
    SegTree(const vector<long long>& a, function<long long(long long, long long)> combine,
            long long identity)
        : Base(a, std::move(combine), identity) {}
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ segment-tree.hpp -o demo && ./demo
int main() {
    vector<long long> a = {5, 2, 4, 7, 1};
    SegTree st(a, [](long long x, long long y) { return min(x, y); }, LLONG_MAX);
    printf("min[0,5)=%lld ", st.query(0, 5));
    st.set(3, -1);
    printf(" -> min[2,5)=%lld", st.query(2, 5));
    SegTree mx(a, [](long long x, long long y) { return max(x, y); }, LLONG_MIN);
    printf("; vị trí đầu >=6: %d\n", mx.findFirst(0, [](long long v) { return v >= 6; }));
    return 0;
}
#endif
