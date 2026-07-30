#pragma once
#include <bits/stdc++.h>
using namespace std;
// LCA (Binary Lifting) — tổ tiên chung gần nhất + tổ tiên thứ k + khoảng cách trên CÂY, tiền xử lý O(n log n).
// Khi dùng: nhiều truy vấn LCA / khoảng cách hai đỉnh / nhảy lên k bậc trên cây tĩnh có gốc.
// ĐPT: build O(n log n), lca/kth/dist O(log n); bộ nhớ O(n log n). Đỉnh 0-based; dựng bằng BFS (không tràn stack).
// Dùng: LCA t(adj, root); t.lca(a, b); t.dist(a, b); t.kth(v, k); // tổ tiên thứ k của v (-1 nếu vượt gốc)
// Bẫy: adj là cây VÔ HƯỚNG (n-1 cạnh); cha của gốc = chính gốc; kth trả -1 nếu k > depth[v].
// CSES: 1135 1136 1687 1688 1705 2101 3114 3149 3409
struct LCA {
    int n, LOG;
    vector<vector<int>> up;   // up[k][v] = tổ tiên thứ 2^k của v
    vector<int> depth;
    // Ý nghĩa: dựng bảng nhảy nhị phân (up) và mảng độ sâu bằng BFS từ gốc.
    // Tham số: adj = danh sách kề cây vô hướng (0-based); root = đỉnh gốc (mặc định 0).
    LCA(const vector<vector<int>>& adj, int root = 0) {
        n = (int)adj.size();
        LOG = 1; while ((1 << LOG) < n) LOG++;
        up.assign(LOG + 1, vector<int>(n, root));
        depth.assign(n, 0);
        vector<char> vis(n, 0);
        queue<int> q; q.push(root); vis[root] = 1; up[0][root] = root;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) if (!vis[v]) { vis[v] = 1; up[0][v] = u; depth[v] = depth[u] + 1; q.push(v); }
        }
        for (int k = 1; k <= LOG; k++)
            for (int v = 0; v < n; v++) up[k][v] = up[k - 1][up[k - 1][v]];
    }
    // Ý nghĩa: tổ tiên thứ k của v (leo k bậc lên phía gốc).
    // Tham số: v = đỉnh xuất phát; k = số bậc cần leo.
    // Trả về: đỉnh tổ tiên thứ k, hoặc -1 nếu k > depth[v] (vượt quá gốc).
    int kth(int v, long long k) {
        if (k > depth[v]) return -1;
        for (int i = 0; i <= LOG; i++) if ((k >> i) & 1) v = up[i][v];
        return v;
    }
    // Ý nghĩa: tổ tiên chung gần nhất của a và b.
    // Tham số: a, b = hai đỉnh bất kỳ.
    // Trả về: đỉnh LCA của a và b.
    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        int d = depth[a] - depth[b];
        for (int i = 0; i <= LOG; i++) if ((d >> i) & 1) a = up[i][a];
        if (a == b) return a;
        for (int i = LOG; i >= 0; i--) if (up[i][a] != up[i][b]) { a = up[i][a]; b = up[i][b]; }
        return up[0][a];
    }
    // Ý nghĩa: khoảng cách (số cạnh) trên đường đi giữa a và b.
    // Tham số: a, b = hai đỉnh bất kỳ.
    // Trả về: depth[a] + depth[b] - 2*depth[lca(a, b)].
    int dist(int a, int b) { return depth[a] + depth[b] - 2 * depth[lca(a, b)]; }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ lca-binary-lifting.hpp -o demo && ./demo
int main() {
    // Cây (gốc 0): cạnh 0-1, 0-2, 1-3, 1-4
    vector<vector<int>> adj = {{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
    LCA t(adj, 0);
    printf("lca(3,4)=%d  lca(3,2)=%d  dist(3,2)=%d  kth(3,1)=%d\n",
           t.lca(3, 4), t.lca(3, 2), t.dist(3, 2), t.kth(3, 1));   // 1  0  3  1
    return 0;
}
#endif
