#include <bits/stdc++.h>
using namespace std;

struct Edge { int to, cap, cost; };
static vector<Edge> edges;
static vector<vector<int>> g;

// Thêm cặp cạnh cho mạng luồng: cạnh xuôi (cap, cost) và cạnh ngược (0, -cost).
void addEdge(int a, int b, int cap, int cost) {
    g[a].push_back((int)edges.size()); edges.push_back({b, cap, cost});
    g[b].push_back((int)edges.size()); edges.push_back({a, 0, -cost});
}

int main() {
    int n, m, k;
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;
    int N = n + 1;
    g.assign(N, {});

    // Mỗi teleporter a->b thành một cạnh capacity = 1, cost = 1: dùng tối đa một
    // lần, và mỗi lần dùng tốn 1 coin. Cạnh xuôi thứ i nằm ở chỉ số 2*i.
    vector<int> ea(m), eb(m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &ea[i], &eb[i]);
        addEdge(ea[i], eb[i], 1, 1);
    }

    const long long INF = LLONG_MAX / 4;
    int s = 1, t = n;                                 // source = phòng 1, sink = phòng n
    vector<long long> h(N, 0), dist(N);               // h[] = Johnson potentials
    vector<int> pe(N);                                // cạnh cha trên đường ngắn nhất
    int flow = 0;
    long long cost = 0;

    // Successive Shortest Paths: mỗi vòng tìm đường tăng luồng ngắn nhất theo cost
    // bằng Dijkstra trên reduced cost, rồi bơm 1 đơn vị luồng, cho đến khi đủ k.
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
                    // reduced cost = c(u,v) + h[u] - h[v] >= 0, đảm bảo không âm
                    long long nd = d + e.cost + h[u] - h[e.to];
                    if (nd < dist[e.to]) {
                        dist[e.to] = nd;
                        pe[e.to] = id;
                        pq.push({nd, e.to});
                    }
                }
            }
        }
        if (dist[t] >= INF) break;                    // không thể bơm thêm luồng

        // Cập nhật potentials: sau đó h[t] chính là chi phí thực sự của đường ngắn nhất
        for (int i = 1; i < N; i++) if (dist[i] < INF) h[i] += dist[i];

        int f = k - flow;                             // tìm bottleneck (thực tế luôn = 1)
        for (int v = t; v != s; ) { int id = pe[v]; f = min(f, edges[id].cap); v = edges[id ^ 1].to; }
        for (int v = t; v != s; ) { int id = pe[v]; edges[id].cap -= f; edges[id ^ 1].cap += f; v = edges[id ^ 1].to; }
        flow += f;
        cost += (long long)f * h[t];                  // cộng dồn chi phí đường vừa bơm
    }

    if (flow < k) { printf("-1\n"); return 0; }       // max-flow < k -> không đủ k đường

    printf("%lld\n", cost);                           // tổng số coin = chi phí luồng

    // Phân rã luồng thành k đường đi: cạnh gốc i được dùng khi cap forward = 0.
    // Gom các cạnh ra đã dùng của mỗi đỉnh vào nxt[u].
    vector<vector<int>> nxt(N);
    for (int i = 0; i < m; i++) if (edges[2 * i].cap == 0) nxt[ea[i]].push_back(eb[i]);

    // Lặp k lần: từ phòng 1, mỗi bước lấy một cạnh ra chưa dùng (con trỏ ptr[u])
    // đi tiếp cho đến khi tới phòng n; bảo toàn luồng bảo đảm luôn có cạnh ra.
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
