#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m, q;
    if (scanf("%d %d %d", &n, &m, &q) != 3) return 0;

    // Đọc cạnh và dựng danh sách kề dạng CSR để BFS nhanh.
    vector<int> deg(n + 1, 0);
    vector<int> ea(m), eb(m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &ea[i], &eb[i]);
        deg[ea[i]]++;
        deg[eb[i]]++;
    }
    vector<int> head(n + 2, 0);
    for (int i = 1; i <= n; i++) head[i + 1] = head[i] + deg[i];
    vector<int> adj(2 * m);
    {
        vector<int> pos(n + 1);
        for (int i = 1; i <= n; i++) pos[i] = head[i];
        for (int i = 0; i < m; i++) {
            adj[pos[ea[i]]++] = eb[i];
            adj[pos[eb[i]]++] = ea[i];
        }
    }

    // Đọc truy vấn, gom theo đỉnh nguồn để mỗi nguồn chỉ BFS một lần.
    vector<int> qa(q), qb(q);
    vector<long long> qx(q);
    vector<vector<int>> bysrc(n + 1);
    for (int i = 0; i < q; i++) {
        long long x;
        scanf("%d %d %lld", &qa[i], &qb[i], &x);
        qx[i] = x;
        bysrc[qa[i]].push_back(i);
    }

    vector<char> ans(q, 0);

    // Trạng thái parity graph: state = đỉnh * 2 + parity (số bước đã đi mod 2).
    const int INF = 0x3f3f3f3f;
    vector<int> dist(2 * (n + 1), INF);
    vector<int> que(2 * (n + 1));

    for (int s = 1; s <= n; s++) {
        if (bysrc[s].empty()) continue;

        // BFS trên parity graph bắt đầu từ (s, parity 0).
        fill(dist.begin(), dist.end(), INF);
        int qh = 0, qt = 0;
        int start = s * 2;              // trạng thái (s, parity 0)
        dist[start] = 0;
        que[qt++] = start;

        while (qh < qt) {
            int cur = que[qh++];
            int u = cur >> 1;
            int np = (cur & 1) ^ 1;     // parity của đỉnh kề (đi thêm 1 cạnh)
            int du = dist[cur];
            for (int e = head[u]; e < head[u + 1]; e++) {
                int state = adj[e] * 2 + np;
                if (dist[state] == INF) {
                    dist[state] = du + 1;
                    que[qt++] = state;
                }
            }
        }

        // dist[b*2 + (x&1)] là độ dài walk ngắn nhất tới b cùng parity với x.
        // Có walk độ dài đúng x  <=>  độ dài ngắn nhất cùng parity đó <= x.
        for (int idx : bysrc[s]) {
            long long x = qx[idx];
            int d = dist[qb[idx] * 2 + (int)(x & 1)];
            ans[idx] = (d != INF && (long long)d <= x) ? 1 : 0;
        }
    }

    string out;
    out.reserve((size_t)q * 4);
    for (int i = 0; i < q; i++) out += (ans[i] ? "YES\n" : "NO\n");
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
