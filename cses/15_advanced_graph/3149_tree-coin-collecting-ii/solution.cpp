#include <bits/stdc++.h>
using namespace std;

// Công thức mỗi truy vấn: answer(a, b) = 2*W_C + f(a) + f(b) - tcedges(a, b)
//   W_C          = số cạnh của Steiner tree của các đỉnh coin (T_C)
//   f(x)         = khoảng cách từ x tới đỉnh gần nhất thuộc T_C (multi-source BFS)
//   tcedges(a,b) = số cạnh thuộc T_C nằm trên đường a-b = g[a] + g[b] - 2*g[lca]
// Mọi lần duyệt cây đều dùng BFS (không đệ quy) để tránh tràn stack.
int main() {
    int n, q;
    if (scanf("%d %d", &n, &q) != 2) return 0;

    vector<int> coin(n + 1);
    for (int i = 1; i <= n; i++) scanf("%d", &coin[i]);

    vector<int> adjHead(n + 1, -1), adjNext(2 * (n > 0 ? n - 1 : 0) + 2), adjTo(2 * (n > 0 ? n - 1 : 0) + 2);
    int ecnt = 0;
    auto addEdge = [&](int u, int v) {
        adjTo[ecnt] = v; adjNext[ecnt] = adjHead[u]; adjHead[u] = ecnt++;
    };
    for (int i = 0; i < n - 1; i++) {
        int a, b; scanf("%d %d", &a, &b);
        addEdge(a, b);
        addEdge(b, a);
    }

    // BFS từ gốc = 1 để lấy cha (par), độ sâu (depth) và thứ tự duyệt BFS
    vector<int> par(n + 1, 0), depth(n + 1, 0), order;
    order.reserve(n);
    {
        vector<char> vis(n + 1, 0);
        vector<int> Q(n);
        int qh = 0, qt = 0;
        Q[qt++] = 1; vis[1] = 1; par[1] = 0; depth[1] = 0;
        while (qh < qt) {
            int u = Q[qh++];
            order.push_back(u);
            for (int e = adjHead[u]; e != -1; e = adjNext[e]) {
                int w = adjTo[e];
                if (!vis[w]) { vis[w] = 1; par[w] = u; depth[w] = depth[u] + 1; Q[qt++] = w; }
            }
        }
    }

    // Đếm số coin trong mỗi subtree (duyệt ngược thứ tự BFS: con trước cha)
    vector<int> cnt(n + 1, 0);
    int total = 0;
    for (int i = 1; i <= n; i++) { cnt[i] = coin[i]; total += coin[i]; }
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int u = order[i];
        if (par[u]) cnt[par[u]] += cnt[u];
    }

    // Xác định cạnh / đỉnh thuộc T_C: cạnh (u, par[u]) thuộc T_C khi cả hai phía
    // đều có coin, tức 0 < cnt[u] < total. W_C = tổng số cạnh như vậy.
    vector<char> inTC(n + 1, 0);
    vector<char> tcEdge(n + 1, 0); // tcEdge[u] = 1 nếu cạnh (u, par[u]) thuộc T_C
    long long Wc = 0;
    for (int u = 1; u <= n; u++) if (coin[u]) inTC[u] = 1;
    for (int u = 1; u <= n; u++) {
        if (par[u] != 0 && cnt[u] > 0 && cnt[u] < total) {
            tcEdge[u] = 1;
            inTC[u] = 1;
            inTC[par[u]] = 1;
            Wc++;
        }
    }

    // g[x] = số cạnh T_C trên đường từ gốc tới x (tiền tố theo thứ tự BFS: cha trước con)
    vector<int> g(n + 1, 0);
    for (int u : order) {
        if (par[u] != 0) g[u] = g[par[u]] + (tcEdge[u] ? 1 : 0);
    }

    // f[x] = khoảng cách tới đỉnh gần nhất thuộc T_C (multi-source BFS)
    vector<int> f(n + 1, -1);
    {
        vector<int> Q(n);
        int qh = 0, qt = 0;
        for (int u = 1; u <= n; u++) if (inTC[u]) { f[u] = 0; Q[qt++] = u; }
        while (qh < qt) {
            int u = Q[qh++];
            for (int e = adjHead[u]; e != -1; e = adjNext[e]) {
                int w = adjTo[e];
                if (f[w] == -1) { f[w] = f[u] + 1; Q[qt++] = w; }
            }
        }
    }

    // Binary lifting để tính LCA (dùng cho tcedges)
    int LOG = 1;
    while ((1 << LOG) < n) LOG++;
    LOG++;
    vector<vector<int>> up(LOG, vector<int>(n + 1, 0));
    for (int u = 1; u <= n; u++) up[0][u] = par[u]; // par[gốc] = 0 (đỉnh lính canh)
    for (int k = 1; k < LOG; k++)
        for (int u = 1; u <= n; u++)
            up[k][u] = up[k - 1][up[k - 1][u]];

    auto lca = [&](int a, int b) -> int {
        if (depth[a] < depth[b]) swap(a, b);
        int d = depth[a] - depth[b];
        for (int k = 0; k < LOG; k++) if (d & (1 << k)) a = up[k][a];
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; k--)
            if (up[k][a] != up[k][b]) { a = up[k][a]; b = up[k][b]; }
        return up[0][a];
    };

    // Trả lời từng truy vấn theo công thức đã rút gọn về O(1) mỗi truy vấn
    string out;
    out.reserve((size_t)q * 7);
    char buf[24];
    for (int i = 0; i < q; i++) {
        int a, b; scanf("%d %d", &a, &b);
        int l = lca(a, b);
        long long tced = (long long)g[a] + g[b] - 2LL * g[l];
        long long ans = 2LL * Wc + f[a] + f[b] - tced;
        int len = sprintf(buf, "%lld\n", ans);
        out.append(buf, len);
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
