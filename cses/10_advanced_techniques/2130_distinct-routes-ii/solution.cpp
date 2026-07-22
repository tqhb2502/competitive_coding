// Distinct Routes II - CSES 2130
// https://cses.fi/problemset/task/2130
//
// Bai toan: do thi co huong n dinh, m canh (teleporter). Moi teleporter dung
// nhieu nhat 1 lan, moi lan dung ton 1 coin. Can di tu phong 1 den phong n
// dung k lan (k duong di canh-doi-nhau / edge-disjoint), sao cho tong so
// teleporter su dung (tong so coin) la nho nhat. In tong coin va k duong di.
//
// Mo hinh: Min-Cost Max-Flow. Moi canh capacity = 1, cost = 1, source = 1,
// sink = n. Tim luong gia tri dung bang k voi chi phi nho nhat (successive
// shortest paths bang Dijkstra + Johnson potentials). Neu max-flow < k -> -1.
// Vi moi cost > 0, luong toi uu khong chua chu trinh -> phan ra chinh xac
// thanh k duong di don gian.

#include <bits/stdc++.h>
using namespace std;

struct Edge { int to, cap, cost; };
static vector<Edge> edges;
static vector<vector<int>> g;

void addEdge(int a, int b, int cap, int cost) {
    g[a].push_back((int)edges.size()); edges.push_back({b, cap, cost});
    g[b].push_back((int)edges.size()); edges.push_back({a, 0, -cost});
}

int main() {
    int n, m, k;
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;
    int N = n + 1;
    g.assign(N, {});
    vector<int> ea(m), eb(m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &ea[i], &eb[i]);
        addEdge(ea[i], eb[i], 1, 1);          // forward edge i stored at index 2*i
    }

    const long long INF = LLONG_MAX / 4;
    int s = 1, t = n;
    vector<long long> h(N, 0), dist(N);
    vector<int> pe(N);
    int flow = 0;
    long long cost = 0;

    while (flow < k) {
        fill(dist.begin(), dist.end(), INF);
        dist[s] = 0;
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (int id : g[u]) {
                Edge &e = edges[id];
                if (e.cap > 0) {
                    long long nd = d + e.cost + h[u] - h[e.to];
                    if (nd < dist[e.to]) {
                        dist[e.to] = nd;
                        pe[e.to] = id;
                        pq.push({nd, e.to});
                    }
                }
            }
        }
        if (dist[t] >= INF) break;                    // khong the bom them luong
        for (int i = 1; i < N; i++) if (dist[i] < INF) h[i] += dist[i];

        int f = k - flow;                             // tim bottleneck (thuc te luon = 1)
        for (int v = t; v != s; ) { int id = pe[v]; f = min(f, edges[id].cap); v = edges[id ^ 1].to; }
        for (int v = t; v != s; ) { int id = pe[v]; edges[id].cap -= f; edges[id ^ 1].cap += f; v = edges[id ^ 1].to; }
        flow += f;
        cost += (long long)f * h[t];                  // h[t] = chi phi ngan nhat thuc su
    }

    if (flow < k) { printf("-1\n"); return 0; }

    printf("%lld\n", cost);

    // Phan ra luong thanh k duong di: canh goc i duoc dung khi cap forward = 0.
    vector<vector<int>> nxt(N);
    for (int i = 0; i < m; i++) if (edges[2 * i].cap == 0) nxt[ea[i]].push_back(eb[i]);
    vector<int> ptr(N, 0);
    for (int p = 0; p < k; p++) {
        vector<int> path;
        path.push_back(1);
        int cur = 1;
        while (cur != n) {
            int v = nxt[cur][ptr[cur]++];
            cur = v;
            path.push_back(cur);
        }
        printf("%d\n", (int)path.size());
        for (int i = 0; i < (int)path.size(); i++)
            printf("%d%c", path[i], i + 1 < (int)path.size() ? ' ' : '\n');
    }
    return 0;
}
