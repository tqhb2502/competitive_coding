// Fixed Length Walk Queries - CSES 3357
// https://cses.fi/problemset/task/3357
//
// Ý tưởng: tồn tại walk độ dài đúng x từ a tới b  <=>  tồn tại walk độ dài L <= x
// với L cùng tính chẵn/lẻ với x (đồ thị liên thông n>=2 nên có thể chèn từng 2 bước
// tại b để nới độ dài). Cần shortest even-walk và shortest odd-walk từ mỗi nguồn,
// tính bằng BFS trên "parity graph" (đỉnh, parity). Xử lý offline gom theo nguồn.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m, q;
    if (scanf("%d %d %d", &n, &m, &q) != 3) return 0;

    // Đọc cạnh và dựng adjacency dạng CSR để BFS nhanh.
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

    // Đọc truy vấn, gom theo đỉnh nguồn.
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

    // Trạng thái parity graph: state = node * 2 + parity.
    const int INF = 0x3f3f3f3f;
    vector<int> dist(2 * (n + 1), INF);
    vector<int> que(2 * (n + 1));

    for (int s = 1; s <= n; s++) {
        if (bysrc[s].empty()) continue;

        fill(dist.begin(), dist.end(), INF);
        int qh = 0, qt = 0;
        int start = s * 2;          // (s, parity 0)
        dist[start] = 0;
        que[qt++] = start;

        while (qh < qt) {
            int cur = que[qh++];
            int u = cur >> 1;
            int np = (cur & 1) ^ 1; // parity của đỉnh kề
            int du = dist[cur];
            for (int e = head[u]; e < head[u + 1]; e++) {
                int state = adj[e] * 2 + np;
                if (dist[state] == INF) {
                    dist[state] = du + 1;
                    que[qt++] = state;
                }
            }
        }

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
