// Flight Route Requests - CSES 1699
// https://cses.fi/problemset/task/1699
//
// Ta được xây cạnh có hướng TÙY Ý. Với mỗi yêu cầu (a,b) chỉ cần b reachable từ a.
// Xét từng weakly-connected component (WCC) của đồ thị yêu cầu:
//   - Nếu WCC acyclic (DAG): cần V_c - 1 cạnh (đường đi Hamilton theo thứ tự topo).
//   - Nếu WCC có chu trình có hướng: cần V_c cạnh (chu trình Hamilton bao phủ tất cả).
// Đáp án = tổng (V_c - [WCC là DAG]) trên mỗi WCC.
// Cài đặt: DSU cho WCC + Kahn's topo sort để phát hiện chu trình. O(n + m).

#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 100005;
int par[MAXN], sz_[MAXN];

int findp(int x) {
    while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
    return x;
}
void uni(int a, int b) {
    a = findp(a); b = findp(b);
    if (a == b) return;
    if (sz_[a] < sz_[b]) swap(a, b);
    par[b] = a;
    sz_[a] += sz_[b];
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<vector<int>> adj(n + 1);      // cạnh có hướng a -> b (cho Kahn)
    vector<int> indeg(n + 1, 0);
    vector<char> appears(n + 1, 0);      // đỉnh có xuất hiện trong yêu cầu (a != b)
    vector<pair<int,int>> edges;
    edges.reserve(m);

    for (int i = 1; i <= n; i++) { par[i] = i; sz_[i] = 1; }

    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        if (a == b) continue;            // a luôn tới được a: bỏ qua, không nối gì
        edges.push_back({a, b});
        adj[a].push_back(b);
        indeg[b]++;
        appears[a] = appears[b] = 1;
    }

    // DSU: tạo các weakly-connected component
    for (auto &e : edges) uni(e.first, e.second);

    // Kahn's topological sort (lặp, không đệ quy)
    queue<int> q;
    for (int v = 1; v <= n; v++)
        if (appears[v] && indeg[v] == 0) q.push(v);

    vector<char> removed(n + 1, 0);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        removed[u] = 1;
        for (int w : adj[u])
            if (--indeg[w] == 0) q.push(w);
    }

    // WCC nào còn đỉnh chưa bị gỡ => WCC đó chứa chu trình có hướng
    vector<char> cyclic(n + 1, 0);
    for (int v = 1; v <= n; v++)
        if (appears[v] && !removed[v]) cyclic[findp(v)] = 1;

    // Cộng theo từng root: sz - (cyclic ? 0 : 1)
    long long ans = 0;
    for (int v = 1; v <= n; v++) {
        if (appears[v] && findp(v) == v) {
            ans += (long long)sz_[v] - (cyclic[v] ? 0 : 1);
        }
    }

    printf("%lld\n", ans);
    return 0;
}
