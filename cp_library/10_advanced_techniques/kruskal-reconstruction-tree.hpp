#pragma once
#include <bits/stdc++.h>
using namespace std;
// Kruskal Reconstruction Tree — mỗi lần hợp của Kruskal thành MỘT node mới; bottleneck(u, v) = val tại LCA, O(m log m).
// Khi dùng: hỏi "cạnh lớn nhất nhỏ nhất" trên đường u-v; ngày sớm nhất hai đỉnh liên thông; truy vấn theo ngưỡng trọng số.
// ĐPT: dựng O(m log m + n log n); bottleneck O(log n); bộ nhớ O(n log n). Lá 0..n-1 là đỉnh gốc, node >= n là node hợp.
// Dùng: KruskalTree kt(n, edges); kt.bottleneck(u, v); // edges là (w, u, v) ĐÃ SẮP theo w tăng dần (hoặc theo thời gian)
// Bẫy: edges PHẢI đưa vào theo thứ tự tăng của w; bottleneck(u,u)=0; khác thành phần trả -1; leaves[node] = số đỉnh gốc dưới node.
// CSES: 2101
struct KruskalTree {
    int n, total, LOG;
    vector<long long> val;              // val[node] = trọng số cạnh đã tạo node (0 với lá)
    vector<int> par, dep, leaves, comp;  // comp[v] = gốc KRT của thành phần chứa v
    vector<vector<int>> up, ch;
    // Ý nghĩa: dựng cây tái tạo Kruskal từ danh sách cạnh đã sắp tăng theo trọng số, kèm bảng LCA nhảy nhị phân.
    // Tham số: n = số đỉnh gốc (0-based); edges = danh sách (w, u, v) ĐÃ SẮP theo w tăng dần.
    KruskalTree(int n, const vector<tuple<long long, int, int>>& edges) : n(n) {
        int mx = 2 * max(1, n);
        val.assign(mx, 0); par.assign(mx, -1); dep.assign(mx, 0); leaves.assign(mx, 0);
        comp.assign(mx, -1); ch.assign(mx, {});
        vector<int> dsu(mx);
        iota(dsu.begin(), dsu.end(), 0);
        function<int(int)> find = [&](int x) { while (dsu[x] != x) x = dsu[x] = dsu[dsu[x]]; return x; };
        total = n;
        for (int v = 0; v < n; v++) leaves[v] = 1;
        for (auto& [w, u, v] : edges) {
            int a = find(u), b = find(v);
            if (a == b) continue;                       // cạnh trong cùng thành phần: bỏ (không tạo node)
            int node = total++;
            val[node] = w;
            par[a] = node; par[b] = node;
            ch[node].push_back(a); ch[node].push_back(b);
            leaves[node] = leaves[a] + leaves[b];
            dsu[a] = node; dsu[b] = node; dsu[node] = node;
        }
        LOG = 1;
        while ((1 << LOG) < max(1, total)) LOG++;
        up.assign(LOG + 1, vector<int>(max(1, total), 0));
        for (int r = 0; r < total; r++) {                // DFS LẶP từ mỗi gốc của rừng KRT
            if (par[r] != -1) continue;
            dep[r] = 0; up[0][r] = r; comp[r] = r;
            vector<int> st = {r};
            while (!st.empty()) {
                int x = st.back(); st.pop_back();
                for (int y : ch[x]) { dep[y] = dep[x] + 1; up[0][y] = x; comp[y] = r; st.push_back(y); }
            }
        }
        for (int k = 1; k <= LOG; k++)
            for (int v = 0; v < total; v++) up[k][v] = up[k - 1][up[k - 1][v]];
    }
    // Ý nghĩa: tổ tiên chung gần nhất của hai node trong cây tái tạo (dùng nhảy nhị phân).
    // Tham số: a, b = hai node của KRT (thường là hai lá = hai đỉnh gốc); PHẢI cùng cây (comp[a] == comp[b]).
    // Trả về: node LCA; nếu a, b khác cây thì giá trị trả về VÔ NGHĨA (hãy dùng bottleneck để được -1).
    int lca(int a, int b) const {
        if (dep[a] < dep[b]) swap(a, b);
        int d = dep[a] - dep[b];
        for (int i = 0; i <= LOG; i++) if ((d >> i) & 1) a = up[i][a];
        if (a == b) return a;
        for (int i = LOG; i >= 0; i--) if (up[i][a] != up[i][b]) { a = up[i][a]; b = up[i][b]; }
        return up[0][a];
    }
    // Ý nghĩa: giá trị "cạnh lớn nhất nhỏ nhất" (bottleneck) trên đường đi giữa u và v.
    // Tham số: u, v = hai đỉnh gốc (0-based).
    // Trả về: val tại LCA (0 nếu u == v), hoặc -1 nếu hai đỉnh khác thành phần.
    long long bottleneck(int u, int v) const {
        if (u == v) return 0;
        if (comp[u] != comp[v]) return -1;
        return val[lca(u, v)];
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ kruskal-reconstruction-tree.hpp -o demo && ./demo
int main() {
    // Cạnh theo thứ tự ngày: ngày 1: 0-1 ; ngày 2: 2-3 ; ngày 3: 1-2 ; đỉnh 4 cô lập
    vector<tuple<long long, int, int>> edges = {{1, 0, 1}, {2, 2, 3}, {3, 1, 2}};
    KruskalTree kt(5, edges);
    printf("bottleneck(0,1)=%lld ; bottleneck(0,3)=%lld ; bottleneck(0,4)=%lld\n",
           kt.bottleneck(0, 1), kt.bottleneck(0, 3), kt.bottleneck(0, 4));   // 1 ; 3 ; -1
    return 0;
}
#endif
