#pragma once
#include <bits/stdc++.h>
using namespace std;
// Offline LCA (Tarjan) — trả lời MỌI truy vấn LCA cùng lúc bằng một lượt DFS + DSU, O((n + q) α(n)).
// Khi dùng: biết trước toàn bộ cặp truy vấn (offline) và muốn nhanh/gọn hơn binary lifting; nền cho khoảng cách cây.
// ĐPT: O((n + q) α(n)); bộ nhớ O(n + q). Đỉnh 0-based; DFS LẶP (không tràn stack); cây phải LIÊN THÔNG.
// Dùng: auto ans = offlineLCA(adj, queries, root); // ans[i] = LCA của queries[i].first và queries[i].second
// Bẫy: adj phải là CÂY liên thông chứa root; truy vấn (u, u) trả về u; đỉnh không tới được từ root -> đáp án -1.
// CSES: 1135 1136 1688
// Ý nghĩa: tính LCA cho mọi truy vấn bằng thuật toán Tarjan offline (DFS lặp + DSU, tô màu khi rời đỉnh).
// Tham số: adj = danh sách kề của cây (vô hướng, 0-based); queries = các cặp đỉnh cần tìm LCA; root = gốc (mặc định 0).
// Trả về: mảng ans cùng cỡ queries, ans[i] = LCA của cặp thứ i (-1 nếu có đỉnh không tới được từ root).
vector<int> offlineLCA(const vector<vector<int>>& adj, const vector<pair<int, int>>& queries, int root = 0) {
    int n = (int)adj.size(), q = (int)queries.size();
    vector<int> ans(q, -1);
    if (n == 0) return ans;
    vector<vector<pair<int, int>>> qat(n);   // qat[u] = {(đỉnh còn lại, id truy vấn)}
    for (int i = 0; i < q; i++) {
        qat[queries[i].first].push_back({queries[i].second, i});
        qat[queries[i].second].push_back({queries[i].first, i});
    }
    vector<int> p(n), sz(n, 1), anc(n), par(n, -1), it(n, 0);
    iota(p.begin(), p.end(), 0);
    iota(anc.begin(), anc.end(), 0);   // anc[gốc tập] = đỉnh cao nhất (gần root nhất) của tập đó
    vector<char> vis(n, 0), black(n, 0);
    function<int(int)> find = [&](int x) { while (p[x] != x) x = p[x] = p[p[x]]; return x; };
    vector<int> st = {root};
    vis[root] = 1;
    while (!st.empty()) {
        int u = st.back();
        if (it[u] < (int)adj[u].size()) {
            int v = adj[u][it[u]++];
            if (vis[v]) continue;                       // bỏ cha và đỉnh đã thăm
            par[v] = u; vis[v] = 1; st.push_back(v);
        } else {
            black[u] = 1;                               // tô màu khi RỜI đỉnh (đã gộp xong mọi con)
            for (auto& [w, qi] : qat[u]) if (black[w]) ans[qi] = anc[find(w)];
            st.pop_back();
            if (par[u] != -1) {                         // gộp u vào cha rồi đặt lại đại diện cao nhất
                int pu = par[u], ru = find(u), rp = find(pu);
                if (ru != rp) {
                    if (sz[ru] > sz[rp]) swap(ru, rp);
                    p[ru] = rp; sz[rp] += sz[ru];
                }
                anc[find(pu)] = pu;
            }
        }
    }
    return ans;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ offline-lca.hpp -o demo && ./demo
int main() {
    // Cây (gốc 0): cạnh 0-1, 0-2, 1-3, 1-4
    vector<vector<int>> adj = {{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
    vector<pair<int, int>> qs = {{3, 4}, {3, 2}, {4, 1}};
    auto ans = offlineLCA(adj, qs, 0);
    printf("LCA(3,4)=%d LCA(3,2)=%d LCA(4,1)=%d\n", ans[0], ans[1], ans[2]);   // 1 0 1
    return 0;
}
#endif
