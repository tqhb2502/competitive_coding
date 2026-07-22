// Critical Cities - CSES 1703
// https://cses.fi/problemset/task/1703
//
// Critical city = đỉnh nằm trên MỌI đường đi từ 1 tới n = DOMINATOR của n.
// Đáp án = chuỗi các tổ tiên của n trên dominator tree (gốc = đỉnh 1).
// Dùng thuật toán Lengauer-Tarjan (link-eval + path compression).
// DFS và path compression đều viết dạng ITERATIVE để tránh tràn stack.

#include <bits/stdc++.h>
using namespace std;

static vector<int> sdom, idomv, lab, anc; // dfs-index-based LT arrays
static vector<int> pathBuf;               // reusable buffer for compress

// Path compression (iterative). Precondition: anc[v] != -1.
static void compress(int v) {
    pathBuf.clear();
    int u = v;
    while (anc[anc[u]] != -1) {
        pathBuf.push_back(u);
        u = anc[u];
    }
    for (int i = (int)pathBuf.size() - 1; i >= 0; --i) {
        int x = pathBuf[i];
        if (sdom[lab[anc[x]]] < sdom[lab[x]]) lab[x] = lab[anc[x]];
        anc[x] = anc[anc[x]];
    }
}

// Eval: trả về đỉnh (dfs-index) có semidominator nhỏ nhất trên đường DSU.
static inline int evalf(int v) {
    if (anc[v] == -1) return v;
    compress(v);
    return lab[v];
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<vector<int>> adj(n + 1), radj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        adj[a].push_back(b);
        radj[b].push_back(a);
    }

    // --- Iterative DFS từ đỉnh 1: đánh số preorder, lưu cha trên cây DFS ---
    vector<int> arr(n + 1, -1); // arr[vertex] = dfs index (-1 nếu không tới được)
    vector<int> rev(n + 1, -1); // rev[dfsIndex] = vertex
    vector<int> par;            // par[dfsIndex] = dfs index của cha (-1 cho gốc)
    par.reserve(n + 1);

    {
        vector<pair<int,int>> st; // (vertex, parentDfsIndex)
        st.reserve(n + 1);
        st.push_back({1, -1});
        int cnt = 0;
        while (!st.empty()) {
            auto [v, p] = st.back();
            st.pop_back();
            if (arr[v] != -1) continue;
            arr[v] = cnt;
            rev[cnt] = v;
            par.push_back(p);
            ++cnt;
            for (int w : adj[v])
                if (arr[w] == -1) st.push_back({w, arr[v]});
        }
    }

    int N = (int)par.size(); // số đỉnh reachable từ gốc

    // --- Lengauer-Tarjan trên không gian dfs-index ---
    sdom.assign(N, 0);
    idomv.assign(N, 0);
    lab.assign(N, 0);
    anc.assign(N, -1);
    for (int i = 0; i < N; ++i) { sdom[i] = i; lab[i] = i; }

    vector<vector<int>> bucket(N);

    for (int i = N - 1; i >= 1; --i) {
        int w = rev[i];
        for (int u : radj[w]) {
            int ju = arr[u];
            if (ju == -1) continue; // predecessor không reachable
            int t = evalf(ju);
            if (sdom[t] < sdom[i]) sdom[i] = sdom[t];
        }
        bucket[sdom[i]].push_back(i);

        // link i vào cha
        anc[i] = par[i];

        // xử lý các đỉnh có semidominator = par[i]
        for (int v : bucket[par[i]]) {
            int u = evalf(v);
            idomv[v] = (sdom[u] == sdom[v]) ? sdom[v] : u;
        }
        bucket[par[i]].clear();
    }

    // chỉnh lại idom
    for (int i = 1; i < N; ++i)
        if (idomv[i] != sdom[i]) idomv[i] = idomv[idomv[i]];
    idomv[0] = 0;

    // --- Thu thập chuỗi dominator của n: đi từ n lên gốc theo idom ---
    vector<int> res;
    int cur = arr[n]; // n chắc chắn reachable
    while (true) {
        res.push_back(rev[cur]);
        if (cur == 0) break;
        cur = idomv[cur];
    }
    sort(res.begin(), res.end());

    printf("%d\n", (int)res.size());
    for (size_t i = 0; i < res.size(); ++i)
        printf("%d%c", res[i], i + 1 == res.size() ? '\n' : ' ');
    return 0;
}
