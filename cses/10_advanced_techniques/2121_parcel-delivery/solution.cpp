// Parcel Delivery - CSES 2121
// https://cses.fi/problemset/task/2121
//
// Min-Cost Max-Flow with a fixed required flow of k units.
// source = city 1, sink = city n. Each route (a,b,r,c) -> edge a->b cap r cost c.
// Successive shortest paths using Dijkstra + Johnson potentials (edge costs >= 0).
// If we cannot push k units of flow, print -1; else print minimum total cost.

#include <bits/stdc++.h>
using namespace std;

const long long INF = (long long)4e18;

struct MCMF {
    int n;
    // Edge arrays; edge e and its reverse are e and e^1.
    vector<int> to, nxt, head;
    vector<long long> cap, cst;

    MCMF(int n_) : n(n_), head(n_, -1) {}

    void addEdge(int u, int v, long long c, long long w) {
        to.push_back(v); cap.push_back(c); cst.push_back(w); nxt.push_back(head[u]); head[u] = (int)to.size() - 1;
        to.push_back(u); cap.push_back(0); cst.push_back(-w); nxt.push_back(head[v]); head[v] = (int)to.size() - 1;
    }

    // Push up to maxf units of flow from s to t at minimum cost.
    // Returns {flow pushed, total cost}.
    pair<long long, long long> run(int s, int t, long long maxf) {
        long long flow = 0, cost = 0;
        vector<long long> h(n, 0), dist(n);
        vector<int> prevE(n);

        while (flow < maxf) {
            fill(dist.begin(), dist.end(), INF);
            fill(prevE.begin(), prevE.end(), -1);
            dist[s] = 0;
            priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
            pq.push({0, s});
            while (!pq.empty()) {
                auto [d, u] = pq.top(); pq.pop();
                if (d > dist[u]) continue;
                for (int e = head[u]; e != -1; e = nxt[e]) {
                    if (cap[e] <= 0) continue;
                    int v = to[e];
                    long long nd = d + cst[e] + h[u] - h[v];
                    if (nd < dist[v]) {
                        dist[v] = nd;
                        prevE[v] = e;
                        pq.push({nd, v});
                    }
                }
            }
            if (dist[t] == INF) break; // sink unreachable

            for (int i = 0; i < n; i++)
                if (dist[i] < INF) h[i] += dist[i];

            // Find bottleneck along the shortest path.
            long long push = maxf - flow;
            for (int v = t; v != s; ) {
                int e = prevE[v];
                push = min(push, cap[e]);
                v = to[e ^ 1];
            }
            // Apply the flow.
            for (int v = t; v != s; ) {
                int e = prevE[v];
                cap[e] -= push;
                cap[e ^ 1] += push;
                v = to[e ^ 1];
            }
            flow += push;
            cost += push * h[t]; // h[t] is now the real shortest distance s->t
        }
        return {flow, cost};
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    MCMF g(n);
    for (int i = 0; i < m; i++) {
        int a, b; long long r, c;
        cin >> a >> b >> r >> c;
        g.addEdge(a - 1, b - 1, r, c);
    }

    auto [flow, cost] = g.run(0, n - 1, k);

    if (flow < k) cout << -1 << '\n';
    else cout << cost << '\n';

    return 0;
}
