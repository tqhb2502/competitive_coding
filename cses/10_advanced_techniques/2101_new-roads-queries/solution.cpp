// New Roads Queries — CSES 2101
// https://cses.fi/problemset/task/2101
//
// Kruskal Reconstruction Tree + LCA (binary lifting).
// Xu ly cac canh theo thu tu ngay 1..m. Moi lan hop nhat hai thanh phan tao mot
// nut trong moi w voi val[w] = ngay, la cha cua hai goc cu. Dap an cho truy van
// (a,b) = val[LCA(a,b)], hoac -1 neu a,b khong cung mot cay (khong bao gio noi).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    int maxNodes = 2 * n + 1;           // la 1..n, noi <= n-1 nut trong -> <= 2n-1
    vector<int> par(maxNodes, 0);       // cha trong cay tai tao (0 = null)
    vector<int> val(maxNodes, 0);       // ngay hop nhat cho nut trong; la = 0
    vector<int> f(maxNodes);            // DSU tim dai dien (dinh cao nhat) hien tai
    for (int i = 0; i < maxNodes; ++i) f[i] = i;

    // find lap voi nen duong (path compression)
    auto find = [&](int x) {
        int r = x;
        while (f[r] != r) r = f[r];
        while (f[x] != r) { int nx = f[x]; f[x] = r; x = nx; }
        return r;
    };

    int cnt = n;                        // so nut hien co; nut trong moi = ++cnt
    for (int d = 1; d <= m; ++d) {
        int a, b;
        cin >> a >> b;
        int ra = find(a), rb = find(b);
        if (ra != rb) {
            int w = ++cnt;
            val[w] = d;
            par[ra] = w;
            par[rb] = w;
            f[ra] = w;
            f[rb] = w;
            f[w] = w;
        }
    }

    const int LOG = 20;                 // 2^20 > 2*2e5
    vector<int> depth(cnt + 1, 0);
    vector<vector<int>> up(LOG, vector<int>(cnt + 1, 0));

    // depth: cha luon co chi so lon hon con => duyet giam dan
    for (int v = cnt; v >= 1; --v) {
        if (par[v] == 0) depth[v] = 0;
        else depth[v] = depth[par[v]] + 1;
        up[0][v] = par[v];
    }
    for (int k = 1; k < LOG; ++k)
        for (int v = 1; v <= cnt; ++v)
            up[k][v] = up[k - 1][up[k - 1][v]];

    auto lca = [&](int a, int b) -> int {
        if (depth[a] < depth[b]) swap(a, b);
        int diff = depth[a] - depth[b];
        for (int k = 0; k < LOG; ++k)
            if (diff & (1 << k)) a = up[k][a];
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; --k)
            if (up[k][a] != up[k][b]) { a = up[k][a]; b = up[k][b]; }
        return par[a];
    };

    string out;
    out.reserve(q * 3);
    for (int i = 0; i < q; ++i) {
        int a, b;
        cin >> a >> b;
        if (find(a) != find(b)) {
            out += "-1\n";
        } else {
            out += to_string(val[lca(a, b)]);
            out += '\n';
        }
    }
    cout << out;
    return 0;
}
