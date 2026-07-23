#include <bits/stdc++.h>
using namespace std;

const long long INF = (long long)4e18;

// Min-Cost Max-Flow (MCMF) theo thuật toán Successive Shortest Paths.
struct MCMF {
    int n;
    // Danh sách cạnh dạng forward-star; cạnh e và cạnh ngược của nó là e và e^1.
    vector<int> to, nxt, head;
    vector<long long> cap, cst;

    MCMF(int n_) : n(n_), head(n_, -1) {}

    // Thêm cạnh có hướng u -> v (cap c, cost w) kèm cạnh residual v -> u (cap 0, cost -w).
    void addEdge(int u, int v, long long c, long long w) {
        to.push_back(v); cap.push_back(c); cst.push_back(w); nxt.push_back(head[u]); head[u] = (int)to.size() - 1;
        to.push_back(u); cap.push_back(0); cst.push_back(-w); nxt.push_back(head[v]); head[v] = (int)to.size() - 1;
    }

    // Đẩy tối đa maxf đơn vị luồng từ s tới t với tổng chi phí nhỏ nhất.
    // Trả về {lượng luồng đã đẩy, tổng chi phí}.
    pair<long long, long long> run(int s, int t, long long maxf) {
        long long flow = 0, cost = 0;
        vector<long long> h(n, 0), dist(n); // h: thế năng Johnson để khử cạnh chi phí âm
        vector<int> prevE(n);

        while (flow < maxf) {
            // Dijkstra trên "reduced cost" = cst[e] + h[u] - h[v] (luôn >= 0).
            fill(dist.begin(), dist.end(), INF);
            fill(prevE.begin(), prevE.end(), -1);
            dist[s] = 0;
            priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
            pq.push({0, s});
            while (!pq.empty()) {
                auto [d, u] = pq.top(); pq.pop();
                if (d > dist[u]) continue;
                for (int e = head[u]; e != -1; e = nxt[e]) {
                    if (cap[e] <= 0) continue; // chỉ đi qua cạnh còn capacity
                    int v = to[e];
                    long long nd = d + cst[e] + h[u] - h[v];
                    if (nd < dist[v]) {
                        dist[v] = nd;
                        prevE[v] = e;
                        pq.push({nd, v});
                    }
                }
            }
            if (dist[t] == INF) break; // không còn đường tới sink -> dừng

            // Cập nhật thế năng: h[v] += dist[v], khi đó h[t] là khoảng cách thật s -> t.
            for (int i = 0; i < n; i++)
                if (dist[i] < INF) h[i] += dist[i];

            // Tìm bottleneck (capacity nhỏ nhất) dọc đường ngắn nhất vừa tìm.
            long long push = maxf - flow;
            for (int v = t; v != s; ) {
                int e = prevE[v];
                push = min(push, cap[e]);
                v = to[e ^ 1];
            }
            // Đẩy luồng dọc đường: giảm cạnh xuôi, tăng cạnh ngược.
            for (int v = t; v != s; ) {
                int e = prevE[v];
                cap[e] -= push;
                cap[e ^ 1] += push;
                v = to[e ^ 1];
            }
            flow += push;
            cost += push * h[t]; // cộng dồn chi phí = lượng đẩy * độ dài đường rẻ nhất
        }
        return {flow, cost};
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;

    // source = thành phố 1 (đỉnh 0), sink = thành phố n (đỉnh n-1).
    MCMF g(n);
    for (int i = 0; i < m; i++) {
        int a, b; long long r, c;
        cin >> a >> b >> r >> c;
        g.addEdge(a - 1, b - 1, r, c);
    }

    // Cần đẩy đúng k đơn vị luồng với chi phí cực tiểu.
    auto [flow, cost] = g.run(0, n - 1, k);

    // Không đẩy đủ k -> không thể giao đủ; ngược lại in tổng chi phí nhỏ nhất.
    if (flow < k) cout << -1 << '\n';
    else cout << cost << '\n';

    return 0;
}
