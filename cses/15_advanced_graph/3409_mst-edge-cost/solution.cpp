// MST Edge Cost - CSES 3409
// https://cses.fi/problemset/task/3409
//
// Với mỗi cạnh, tính chi phí MST bắt buộc chứa cạnh đó.
// ans(e=(a,b,w)) = T + w - maxEdgeOnPath(a,b trên MST),
// với T là tổng trọng số của một MST bất kỳ.
// Dùng Kruskal + Binary Lifting (LCA) để truy vấn cạnh lớn nhất trên đường đi.

#include <bits/stdc++.h>
using namespace std;

static const int LOG = 17;

int par[100005];
int findp(int x) {
    while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
    return x;
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<int> ea(m), eb(m), ew(m);
    for (int i = 0; i < m; i++)
        scanf("%d %d %d", &ea[i], &eb[i], &ew[i]);

    // --- Kruskal: dựng MST, tính tổng trọng số T ---
    vector<int> idx(m);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(),
         [&](int x, int y) { return ew[x] < ew[y]; });

    for (int i = 1; i <= n; i++) par[i] = i;

    vector<vector<pair<int,int>>> adj(n + 1); // (đỉnh kề, trọng số)
    long long T = 0;
    for (int id : idx) {
        int u = findp(ea[id]), v = findp(eb[id]);
        if (u != v) {
            par[u] = v;
            adj[ea[id]].push_back({eb[id], ew[id]});
            adj[eb[id]].push_back({ea[id], ew[id]});
            T += ew[id];
        }
    }

    // --- Dựng bảng binary lifting bằng DFS lặp (gốc = 1) ---
    vector<int> depth(n + 1, 0);
    vector<array<int, LOG>> up(n + 1);
    vector<array<int, LOG>> mx(n + 1);
    // vector<array> đã value-init về 0, nên up[0], mx[0] = 0 (an toàn cho gốc).

    vector<char> vis(n + 1, 0);
    {
        vector<int> stk;
        stk.reserve(n);
        stk.push_back(1);
        vis[1] = 1;
        depth[1] = 0;
        // up[1], mx[1] đã = 0
        while (!stk.empty()) {
            int u = stk.back(); stk.pop_back();
            for (auto &pr : adj[u]) {
                int v = pr.first, w = pr.second;
                if (!vis[v]) {
                    vis[v] = 1;
                    depth[v] = depth[u] + 1;
                    up[v][0] = u;
                    mx[v][0] = w;
                    stk.push_back(v);
                }
            }
        }
    }

    for (int k = 1; k < LOG; k++) {
        for (int v = 1; v <= n; v++) {
            int mid = up[v][k - 1];
            up[v][k] = up[mid][k - 1];
            mx[v][k] = max(mx[v][k - 1], mx[mid][k - 1]);
        }
    }

    // Cạnh trọng số lớn nhất trên đường đi a..b trên cây MST.
    auto pathMax = [&](int a, int b) -> int {
        int res = 0;
        if (depth[a] < depth[b]) swap(a, b);
        int diff = depth[a] - depth[b];
        for (int k = 0; k < LOG; k++) {
            if (diff & (1 << k)) {
                res = max(res, mx[a][k]);
                a = up[a][k];
            }
        }
        if (a == b) return res;
        for (int k = LOG - 1; k >= 0; k--) {
            if (up[a][k] != up[b][k]) {
                res = max(res, mx[a][k]);
                res = max(res, mx[b][k]);
                a = up[a][k];
                b = up[b][k];
            }
        }
        res = max(res, mx[a][0]);
        res = max(res, mx[b][0]);
        return res;
    };

    // --- Trả lời từng cạnh theo thứ tự nhập ---
    string out;
    out.reserve((size_t)m * 8);
    char buf[24];
    for (int i = 0; i < m; i++) {
        long long ans = T + (long long)ew[i] - (long long)pathMax(ea[i], eb[i]);
        int len = sprintf(buf, "%lld\n", ans);
        out.append(buf, len);
    }
    fputs(out.c_str(), stdout);
    return 0;
}
