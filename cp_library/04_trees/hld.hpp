#pragma once
#include <bits/stdc++.h>
using namespace std;
// HLD (Heavy-Light Decomposition) — truy vấn/cập nhật trên ĐƯỜNG ĐI và CÂY CON của cây, O(log^2 n) mỗi thao tác.
// Khi dùng: cập nhật điểm + hỏi TỔNG trên đường u-v (theo đỉnh) hoặc cây con; đổi phép gộp segtree sang max/min tùy bài.
// ĐPT: build O(n), update O(log n), queryPath/querySubtree O(log^2 n); bộ nhớ O(n). Đỉnh 0-based; segtree tổng lồng sẵn.
// Dùng: HLD t(n); t.addEdge(u,v); t.build(root); t.update(v,val); t.queryPath(u,v); t.querySubtree(v);
// Bẫy: giá trị đặt trên ĐỈNH (path gồm cả hai đầu và LCA); dựng bằng DFS LẶP; đổi '+' trong segtree để làm max/min.
// CSES: 2134
struct HLD {
    int n, timer = 0, segn;
    vector<vector<int>> adj;
    vector<int> par, depth, heavy, head, pos, sz;
    vector<long long> seg;
    // Ý nghĩa: khởi tạo HLD cho cây n đỉnh, cấp phát các mảng và đặt giá trị mặc định.
    // Tham số: n = số đỉnh (đỉnh 0-based).
    explicit HLD(int n) : n(n), adj(n), par(n, -1), depth(n, 0), heavy(n, -1), head(n, 0), pos(n, 0), sz(n, 1) {}
    // Ý nghĩa: thêm cạnh vô hướng (u, v) vào danh sách kề của cây.
    // Tham số: u, v = hai đỉnh đầu mút của cạnh.
    void addEdge(int u, int v) { adj[u].push_back(v); adj[v].push_back(u); }
    // Ý nghĩa: phân rã cây bằng DFS lặp — tính par/depth/sz/heavy rồi gán head/pos theo chuỗi nặng, dựng segtree rỗng.
    // Tham số: root = đỉnh gốc của cây (mặc định 0).
    void build(int root = 0) {
        vector<int> order, st = {root};
        vector<char> vis(n, 0); vis[root] = 1; par[root] = -1;
        while (!st.empty()) {
            int u = st.back(); st.pop_back(); order.push_back(u);
            for (int v : adj[u]) if (!vis[v]) { vis[v] = 1; par[v] = u; depth[v] = depth[u] + 1; st.push_back(v); }
        }
        for (int i = n - 1; i >= 0; i--) {   // cỡ cây con + con nặng
            int u = order[i], mx = 0;
            for (int v : adj[u]) if (v != par[u]) { sz[u] += sz[v]; if (sz[v] > mx) { mx = sz[v]; heavy[u] = v; } }
        }
        vector<int> hs = {root}; head[root] = root;   // gán head/pos: đi dọc chuỗi nặng
        while (!hs.empty()) {
            int h = hs.back(); hs.pop_back();
            for (int v = h; v != -1; v = heavy[v]) {
                head[v] = head[h];
                pos[v] = timer++;
                for (int w : adj[v]) if (w != par[v] && w != heavy[v]) { head[w] = w; hs.push_back(w); }
            }
        }
        segn = n; seg.assign(2 * segn, 0);
    }
    // Ý nghĩa: gán a[i] = val trong segtree tổng và cập nhật các nút cha dọc lên gốc.
    // Tham số: i = vị trí lá (theo pos, 0-based); val = giá trị mới.
    void segUpdate(int i, long long val) {   // gán a[i] = val
        for (seg[i += segn] = val, i >>= 1; i; i >>= 1) seg[i] = seg[2 * i] + seg[2 * i + 1];
    }
    // Ý nghĩa: tính tổng các phần tử trên đoạn đóng [l, r] của segtree.
    // Tham số: l, r = biên trái/phải (theo pos, 0-based, bao gồm cả hai đầu).
    // Trả về: tổng a[l..r].
    long long segQuery(int l, int r) {       // tổng [l, r] (đóng)
        long long res = 0;
        for (l += segn, r += segn + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res += seg[l++];
            if (r & 1) res += seg[--r];
        }
        return res;
    }
    // Ý nghĩa: đặt giá trị val cho đỉnh v (ánh xạ qua pos[v] rồi cập nhật segtree).
    // Tham số: v = đỉnh cần cập nhật; val = giá trị mới.
    void update(int v, long long val) { segUpdate(pos[v], val); }
    // Ý nghĩa: tổng giá trị các đỉnh trên đường đi u-v, leo từng chuỗi nặng và gộp cả đoạn chứa LCA.
    // Tham số: u, v = hai đỉnh đầu mút của đường đi.
    // Trả về: tổng giá trị mọi đỉnh trên đường u-v (gồm cả hai đầu và LCA).
    long long queryPath(int u, int v) {
        long long res = 0;
        for (; head[u] != head[v]; u = par[head[u]]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res += segQuery(pos[head[u]], pos[u]);
        }
        if (depth[u] > depth[v]) swap(u, v);
        res += segQuery(pos[u], pos[v]);   // đoạn gồm cả LCA
        return res;
    }
    // Ý nghĩa: tổng giá trị mọi đỉnh trong cây con gốc v (nhờ pos liên tiếp trong cây con).
    // Tham số: v = đỉnh gốc của cây con.
    // Trả về: tổng giá trị các đỉnh thuộc cây con của v (gồm cả v).
    long long querySubtree(int v) { return segQuery(pos[v], pos[v] + sz[v] - 1); }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ hld.hpp -o demo && ./demo
int main() {
    // Cây (gốc 0): cạnh 0-1, 0-2, 1-3, 1-4
    HLD t(5);
    t.addEdge(0, 1); t.addEdge(0, 2); t.addEdge(1, 3); t.addEdge(1, 4);
    t.build(0);
    long long val[5] = {10, 20, 30, 40, 50};
    for (int v = 0; v < 5; v++) t.update(v, val[v]);
    printf("path(3,2)=%lld  path(3,4)=%lld  subtree(1)=%lld\n",
           t.queryPath(3, 2), t.queryPath(3, 4), t.querySubtree(1));   // 40+20+10+30=100  40+20+50=110  20+40+50=110
    return 0;
}
#endif
