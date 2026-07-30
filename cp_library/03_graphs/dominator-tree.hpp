#pragma once
#include <bits/stdc++.h>
using namespace std;

// DOMINATOR TREE (cây thống trị) — Lengauer–Tarjan cho đồ thị CÓ HƯỚNG, gốc cố định.
// Khi dùng: tìm các đỉnh nằm trên MỌI đường từ source tới v; control-flow graph; critical vertices.
// ĐPT: O((n + m) log n) với link-eval/path compression dạng đơn giản; bộ nhớ O(n + m).
// Dùng: DominatorTree d(n); d.addEdge(u,v); d.build(source);
//       d.idom[v], d.dominates(u,v), d.dominatorChain(v), d.tree[u].
// Quy ước: đỉnh 0-based; idom[source] = source; đỉnh không tới được có idom = -1.
// Bẫy: dominance chỉ xét các đường bắt đầu tại source; phải gọi lại build sau khi thêm cạnh.
// CSES: 1703
struct DominatorTree {
    int n;
    vector<vector<int>> adj;
    vector<int> idom;                // immediate dominator theo số đỉnh gốc
    vector<vector<int>> tree;        // các con trong dominator tree

private:
    int source_ = -1;
    vector<int> dfsId_, vertex_, parent_, semi_, ancestor_, label_, idomDfs_;
    vector<vector<int>> pred_, bucket_;
    vector<int> tin_, tout_, compressPath_;

    // Ý nghĩa: link-eval của Lengauer–Tarjan; trả đỉnh có semi nhỏ nhất trên
    //          đường DSU từ v lên đại diện. Nén đường hoàn toàn bằng vòng lặp.
    int eval(int v) {
        if (ancestor_[v] == 0) return label_[v];
        compressPath_.clear();
        int x = v;
        while (ancestor_[x] != 0 && ancestor_[ancestor_[x]] != 0) {
            compressPath_.push_back(x);
            x = ancestor_[x];
        }
        for (int i = (int)compressPath_.size() - 1; i >= 0; --i) {
            int y = compressPath_[i];
            int a = ancestor_[y];
            if (semi_[label_[a]] < semi_[label_[y]]) label_[y] = label_[a];
            ancestor_[y] = ancestor_[a];
        }
        return label_[v];
    }

public:
    // Ý nghĩa: khởi tạo đồ thị có hướng n đỉnh, chưa có cạnh.
    // Tham số: n = số đỉnh.
    explicit DominatorTree(int n) : n(n), adj(n), idom(n, -1), tree(n) {}

    // Ý nghĩa: thêm cạnh có hướng u -> v.
    // Tham số: u = đầu cạnh, v = cuối cạnh (0-based).
    void addEdge(int u, int v) { adj[u].push_back(v); }

    // Ý nghĩa: dựng cây dominator của phần đồ thị reachable từ source bằng
    //          Lengauer–Tarjan; toàn bộ DFS và path compression đều không đệ quy.
    // Tham số: source = gốc của flow graph.
    // Trả về: số đỉnh tới được từ source.
    int build(int source) {
        source_ = source;
        fill(idom.begin(), idom.end(), -1);
        tree.assign(n, {});
        tin_.assign(n, -1);
        tout_.assign(n, -1);
        if (source < 0 || source >= n) return 0;

        // DFS preorder lặp: chỉ chỉ số DFS 1..reachable mới tham gia thuật toán.
        dfsId_.assign(n, 0);
        vertex_.assign(n + 1, -1);
        parent_.assign(n + 1, 0);
        vector<int> nextEdge(n, 0), stk;
        stk.push_back(source);
        int reached = 1;
        dfsId_[source] = 1;
        vertex_[1] = source;
        while (!stk.empty()) {
            int u = stk.back();
            if (nextEdge[u] == (int)adj[u].size()) {
                stk.pop_back();
                continue;
            }
            int v = adj[u][nextEdge[u]++];
            if (dfsId_[v] != 0) continue;
            dfsId_[v] = ++reached;
            vertex_[reached] = v;
            parent_[reached] = dfsId_[u];
            stk.push_back(v);
        }

        pred_.assign(reached + 1, {});
        for (int u = 0; u < n; ++u) if (dfsId_[u] != 0) {
            for (int v : adj[u]) if (dfsId_[v] != 0)
                pred_[dfsId_[v]].push_back(dfsId_[u]);
        }
        semi_.resize(reached + 1);
        ancestor_.assign(reached + 1, 0);
        label_.resize(reached + 1);
        idomDfs_.assign(reached + 1, 0);
        bucket_.assign(reached + 1, {});
        compressPath_.reserve(reached);
        for (int i = 1; i <= reached; ++i) semi_[i] = label_[i] = i;

        // Pha semidominator + immediate dominator tạm.
        for (int w = reached; w >= 2; --w) {
            for (int v : pred_[w]) semi_[w] = min(semi_[w], semi_[eval(v)]);
            bucket_[semi_[w]].push_back(w);
            ancestor_[w] = parent_[w];                // link(parent[w], w)
            int p = parent_[w];
            for (int v : bucket_[p]) {
                int u = eval(v);
                idomDfs_[v] = (semi_[u] < semi_[v] ? u : p);
            }
            bucket_[p].clear();
        }
        // Hiệu chỉnh idom tạm thành idom thật.
        idomDfs_[1] = 1;
        for (int w = 2; w <= reached; ++w)
            if (idomDfs_[w] != semi_[w]) idomDfs_[w] = idomDfs_[idomDfs_[w]];

        for (int i = 1; i <= reached; ++i) {
            int v = vertex_[i];
            idom[v] = vertex_[idomDfs_[i]];
            if (i != 1) tree[idom[v]].push_back(v);
        }

        // Euler interval trên dominator tree để trả lời dominates(u,v) O(1).
        int timer = 0;
        vector<pair<int, int>> walk;
        walk.push_back({source, 0});
        tin_[source] = timer++;
        while (!walk.empty()) {
            int u = walk.back().first;
            int &it = walk.back().second;
            if (it < (int)tree[u].size()) {
                int v = tree[u][it++];
                tin_[v] = timer++;
                walk.push_back({v, 0});
            } else {
                tout_[u] = timer - 1;
                walk.pop_back();
            }
        }
        return reached;
    }

    // Ý nghĩa: kiểm tra v có reachable từ source của lần build gần nhất không.
    bool reachable(int v) const {
        return 0 <= v && v < n && idom[v] != -1;
    }

    // Ý nghĩa: kiểm tra u dominate v, tức mọi đường source -> v đều qua u.
    // Trả về: false nếu u hoặc v không reachable; ngược lại trả kết quả trong O(1).
    bool dominates(int u, int v) const {
        if (!reachable(u) || !reachable(v)) return false;
        return tin_[u] <= tin_[v] && tin_[v] <= tout_[u];
    }

    // Ý nghĩa: liệt kê toàn bộ dominator của v theo thứ tự source -> ... -> v.
    // Trả về: chuỗi tổ tiên trên dominator tree; rỗng nếu v không reachable.
    vector<int> dominatorChain(int v) const {
        if (!reachable(v)) return {};
        vector<int> chain;
        while (true) {
            chain.push_back(v);
            if (idom[v] == v) break;
            v = idom[v];
        }
        reverse(chain.begin(), chain.end());
        return chain;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ dominator-tree.hpp -o demo && ./demo
int main() {
    DominatorTree d(6);
    d.addEdge(0, 1);
    d.addEdge(1, 2); d.addEdge(1, 3);
    d.addEdge(2, 4); d.addEdge(3, 4);
    d.addEdge(4, 1);                    // chu trình; đỉnh 5 không reachable
    assert(d.build(0) == 5);
    assert(d.idom[0] == 0 && d.idom[1] == 0 && d.idom[4] == 1);
    assert(d.dominates(1, 4) && !d.dominates(2, 4) && !d.reachable(5));
    vector<int> critical = d.dominatorChain(4);
    assert((critical == vector<int>{0, 1, 4}));
    cout << "dominator của đỉnh 4:";
    for (int v : critical) cout << ' ' << v;
    cout << '\n';
}
#endif
