#pragma once
#include <bits/stdc++.h>
using namespace std;
// Persistent Segment Tree — path copying, giữ nhiều phiên bản với O(log n) node mới mỗi cập nhật.
// Khi dùng: cần range order statistic/value-sum trên mảng tĩnh hoặc point-assign/range-sum theo version.
// ĐPT: build O(n log n); truy vấn/cập nhật O(log n), copyVersion O(1); bộ nhớ O(n log n + q log n).
// Dùng: PersistentSeg ps(a); ps.kth(l,r,k); ps.sumLessEqual(l,r,x);
//       PersistentArraySum pa(a); int v=pa.copyVersion(0); pa.pointAssign(v,i,x); pa.rangeSum(v,l,r);
// Bẫy: đoạn [l,r), k 1-based; version của PersistentArraySum là 0-based; tổng phải vừa long long.
// CSES: 1737 2184

// Hai API:
//   1) PersistentSeg: version PREFIX theo vị trí, cây trên trục GIÁ TRỊ đã nén.
//   2) PersistentArraySum: cây trên trục VỊ TRÍ, hỗ trợ copy/pointAssign/rangeSum theo version.
// Prefix-persistent tree trên trục giá trị: root[i] chứa a[0..i).
struct PersistentSeg {
    struct Node {
        int l = 0, r = 0, cnt = 0;
        long long sum = 0;
    };
    vector<Node> t;
    vector<int> root;
    vector<long long> vals;
    int n = 0, m = 1;
    bool allPositive = true;

    // Ý nghĩa: tạo node mới với hai con, số phần tử và tổng giá trị cho trước.
    // Trả về: chỉ số node vừa tạo trong t.
    int make(int l, int r, int cnt, long long sum) {
        t.push_back({l, r, cnt, sum});
        return (int)t.size() - 1;
    }

    // Ý nghĩa: tạo version mới từ prev bằng cách chèn một giá trị tại vị trí nén pos.
    // Tham số: prev = root cũ; [lo,hi) = đoạn chỉ số nén; pos/value = vị trí và giá trị cần chèn.
    // Trả về: root mới; root cũ không đổi.
    int insert(int prev, int lo, int hi, int pos, long long value) {
        if (hi - lo == 1)
            return make(0, 0, t[prev].cnt + 1, t[prev].sum + value);
        int mid = (lo + hi) / 2;
        if (pos < mid)
            return make(insert(t[prev].l, lo, mid, pos, value), t[prev].r,
                        t[prev].cnt + 1, t[prev].sum + value);
        return make(t[prev].l, insert(t[prev].r, mid, hi, pos, value),
                    t[prev].cnt + 1, t[prev].sum + value);
    }

    // Ý nghĩa: dựng các version prefix và nén trục giá trị.
    // Tham số: a = mảng đầu vào 0-based; chấp nhận mọi giá trị vừa long long.
    template <class T>
    explicit PersistentSeg(const vector<T>& a) : n((int)a.size()) {
        vals.assign(a.begin(), a.end());
        for (long long x : vals) allPositive &= (x > 0);
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        m = max(1, (int)vals.size());
        t.push_back({0, 0, 0, 0});  // node 0 = null, tự trỏ về 0
        root.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            long long value = (long long)a[i];
            int pos = (int)(lower_bound(vals.begin(), vals.end(), value) - vals.begin());
            root[i + 1] = insert(root[i], 0, m, pos, value);
        }
    }

    // Ý nghĩa: tìm giá trị nhỏ thứ k trong a[l,r) bằng hiệu root[r] - root[l].
    // Tham số: l,r = đoạn nửa mở 0-based; k = hạng 1-based, 1 <= k <= r-l.
    // Trả về: giá trị nhỏ thứ k.
    long long kth(int l, int r, int k) const {
        assert(0 <= l && l <= r && r <= n);
        assert(1 <= k && k <= r - l);
        int u = root[l], v = root[r], lo = 0, hi = m;
        while (hi - lo > 1) {
            int leftCnt = t[t[v].l].cnt - t[t[u].l].cnt;
            int mid = (lo + hi) / 2;
            if (k <= leftCnt) {
                u = t[u].l;
                v = t[v].l;
                hi = mid;
            } else {
                k -= leftCnt;
                u = t[u].r;
                v = t[v].r;
                lo = mid;
            }
        }
        return vals[lo];
    }

    // Ý nghĩa: lấy tổng giá trị trên hiệu hai cây prefix, chỉ xét index nén < bound.
    // Tham số: u,v = root trước/sau; [lo,hi) = đoạn node; bound = số lá đầu cần lấy.
    long long prefixValueSum(int u, int v, int lo, int hi, int bound) const {
        if (bound <= lo) return 0;
        if (hi <= bound) return t[v].sum - t[u].sum;
        int mid = (lo + hi) / 2;
        return prefixValueSum(t[u].l, t[v].l, lo, mid, bound) +
               prefixValueSum(t[u].r, t[v].r, mid, hi, bound);
    }

    // Ý nghĩa: tính tổng mọi a[i] < x với i thuộc [l,r).
    // Trả về: tổng có tính cả số lần lặp của mỗi giá trị.
    long long sumLess(int l, int r, long long x) const {
        assert(0 <= l && l <= r && r <= n);
        int bound = (int)(lower_bound(vals.begin(), vals.end(), x) - vals.begin());
        return prefixValueSum(root[l], root[r], 0, m, bound);
    }

    // Ý nghĩa: tính tổng mọi a[i] <= x với i thuộc [l,r).
    // Trả về: tổng có tính cả số lần lặp của mỗi giá trị.
    long long sumLessEqual(int l, int r, long long x) const {
        assert(0 <= l && l <= r && r <= n);
        int bound = (int)(upper_bound(vals.begin(), vals.end(), x) - vals.begin());
        return prefixValueSum(root[l], root[r], 0, m, bound);
    }

    // Ý nghĩa: tìm tổng dương nhỏ nhất không tạo được từ một tập con của a[l,r).
    // Kỹ thuật: nếu tạo được [1,reach], mọi đồng xu <= reach+1 mở rộng reach bằng tổng của chúng.
    // Tham số: l,r = đoạn nửa mở; yêu cầu tất cả giá trị trong mảng > 0.
    // Trả về: missing subset sum nhỏ nhất; dùng value-sum persistent, O(log V * log tổng).
    long long smallestMissingSubsetSum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        assert(allPositive);
        long long reach = 0;
        while (reach < LLONG_MAX) {
            long long limit = reach + 1;
            long long next = sumLessEqual(l, r, limit);
            if (next <= reach) return limit;
            reach = next;
        }
        return LLONG_MAX;  // chỉ xảy ra nếu đáp án không biểu diễn được bằng long long
    }
};

// Persistent tree trên trục vị trí, tự quản lý danh sách root theo version.
struct PersistentArraySum {
    struct Node {
        int l = 0, r = 0;
        long long sum = 0;
    };
    int n;
    vector<Node> t;
    vector<int> root;  // root[v] = gốc version v (version 0 là mảng ban đầu)

    // Ý nghĩa: cấp phát một node mới và trả chỉ số của nó.
    int make(const Node& node) {
        t.push_back(node);
        return (int)t.size() - 1;
    }

    // Ý nghĩa: dựng cây tổng ban đầu trên đoạn vị trí [lo,hi).
    template <class T>
    int build(int lo, int hi, const vector<T>& a) {
        if (lo == hi) return 0;
        if (hi - lo == 1) return make({0, 0, (long long)a[lo]});
        int mid = (lo + hi) / 2;
        int left = build(lo, mid, a), right = build(mid, hi, a);
        return make({left, right, t[left].sum + t[right].sum});
    }

    // Ý nghĩa: path-copy từ node prev và gán a[pos]=value; các nhánh khác được dùng chung.
    // Trả về: root mới, còn root prev giữ nguyên.
    int update(int prev, int lo, int hi, int pos, long long value) {
        Node cur = t[prev];
        if (hi - lo == 1) {
            cur.sum = value;
            return make(cur);
        }
        int mid = (lo + hi) / 2;
        if (pos < mid) cur.l = update(cur.l, lo, mid, pos, value);
        else cur.r = update(cur.r, mid, hi, pos, value);
        cur.sum = t[cur.l].sum + t[cur.r].sum;
        return make(cur);
    }

    // Ý nghĩa: tính tổng đoạn [ql,qr) trong cây con node quản lý [lo,hi).
    long long query(int node, int lo, int hi, int ql, int qr) const {
        if (qr <= lo || hi <= ql) return 0;
        if (ql <= lo && hi <= qr) return t[node].sum;
        int mid = (lo + hi) / 2;
        return query(t[node].l, lo, mid, ql, qr) +
               query(t[node].r, mid, hi, ql, qr);
    }

    // Ý nghĩa: tạo version 0 từ mảng ban đầu.
    template <class T>
    explicit PersistentArraySum(const vector<T>& a) : n((int)a.size()) {
        t.reserve(max(1, 2 * n));
        t.push_back({0, 0, 0});  // node 0 = null; cũng là root hợp lệ của mảng rỗng
        root.push_back(build(0, n, a));
    }

    // Ý nghĩa: sao chép version source bằng cách dùng chung root, O(1).
    // Trả về: id 0-based của version mới.
    int copyVersion(int source) {
        assert(0 <= source && source < (int)root.size());
        root.push_back(root[source]);
        return (int)root.size() - 1;
    }

    // Ý nghĩa: gán phần tử pos trong version v; chỉ root[v] chuyển sang cây mới.
    // Tham số: v = version 0-based; pos = vị trí 0-based; value = giá trị mới.
    void pointAssign(int v, int pos, long long value) {
        assert(0 <= v && v < (int)root.size());
        assert(0 <= pos && pos < n);
        root[v] = update(root[v], 0, n, pos, value);
    }

    // Ý nghĩa: lấy tổng đoạn [l,r) trên version v.
    long long rangeSum(int v, int l, int r) const {
        assert(0 <= v && v < (int)root.size());
        assert(0 <= l && l <= r && r <= n);
        return query(root[v], 0, n, l, r);
    }

    // Ý nghĩa: trả số version hiện có.
    int versionCount() const { return (int)root.size(); }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ persistent-segment-tree.hpp -o demo && ./demo
int main() {
    vector<int> a = {5, 2, 8, 1, 9, 3};
    PersistentSeg ps(a);
    printf("kth(2) trong [1,5)=%lld, tổng <=5 trong [0,6)=%lld\n",
           ps.kth(1, 5, 2), ps.sumLessEqual(0, 6, 5));  // 2, 11

    vector<int> coins = {1, 2, 2, 7};
    PersistentSeg coinTree(coins);
    printf("missing subset sum trong [0,4)=%lld\n",
           coinTree.smallestMissingSubsetSum(0, 4));   // 6

    PersistentArraySum versions(vector<long long>{2, 1, 3});
    int copied = versions.copyVersion(0);
    versions.pointAssign(0, 1, 10);
    printf("version 0 sum=%lld, bản copy %d sum=%lld\n",
           versions.rangeSum(0, 0, 3), copied, versions.rangeSum(copied, 0, 3));  // 15, 6
    return 0;
}
#endif
