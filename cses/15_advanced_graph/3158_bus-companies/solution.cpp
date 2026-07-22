// Bus Companies - CSES 3158
// https://cses.fi/problemset/task/3158
//
// Mô hình hóa mỗi bus company bằng một virtual node (nút ảo).
// Với công ty i (giá c_i, tập thành phố S_i):
//   - cạnh  a -> (n+i)  trọng số c_i  (lên xe / mua vé),  với mọi a thuộc S_i
//   - cạnh  (n+i) -> a  trọng số 0    (xuống xe miễn phí)
// Sau đó chạy Dijkstra từ thành phố 1 (Syrjälä).

#include <bits/stdc++.h>
using namespace std;

static char ibuf[1 << 22];
static int ipos = 0, ilen = 0;

static inline int readChar() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen == 0) return -1;
    }
    return ibuf[ipos++];
}

static inline int readInt() {
    int c = readChar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == -1) return 0;
        c = readChar();
    }
    int sgn = 1;
    if (c == '-') { sgn = -1; c = readChar(); }
    long long x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = readChar(); }
    return (int)(sgn * x);
}

int main() {
    int n = readInt();
    int m = readInt();

    // Nodes: 1..n cities, n+1..n+m company virtual nodes.
    int N = n + m;

    vector<int> cost(m + 1);
    for (int i = 1; i <= m; i++) cost[i] = readInt();

    // Build adjacency list.
    vector<vector<pair<int, long long>>> adj(N + 1);
    for (int i = 1; i <= m; i++) {
        int k = readInt();
        long long c = cost[i];
        int cnode = n + i;
        adj[cnode].reserve(k);
        while (k--) {
            int a = readInt();
            adj[a].push_back({cnode, c});   // board bus (pay c)
            adj[cnode].push_back({a, 0});    // alight (free)
        }
    }

    const long long INF = LLONG_MAX;
    vector<long long> dist(N + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> pq;

    dist[1] = 0;
    pq.push({0, 1});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (auto& e : adj[u]) {
            int v = e.first;
            long long nd = d + e.second;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }

    // Output cheapest cost to cities 1..n.
    string out;
    out.reserve(n * 7);
    for (int i = 1; i <= n; i++) {
        long long d = (dist[i] == INF) ? -1 : dist[i];
        out += to_string(d);
        out += (i == n) ? '\n' : ' ';
    }
    fputs(out.c_str(), stdout);
    return 0;
}
