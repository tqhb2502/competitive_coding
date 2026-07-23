#include <bits/stdc++.h>
using namespace std;

// Fast IO: đọc từng ký tự từ buffer để tăng tốc do lượng số đọc vào lớn.
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

    // Đỉnh 1..n là các thành phố, đỉnh n+1..n+m là nút ảo của từng công ty.
    int N = n + m;

    // Đọc giá vé c_i của mỗi công ty.
    vector<int> cost(m + 1);
    for (int i = 1; i <= m; i++) cost[i] = readInt();

    // Dựng danh sách kề của đồ thị mở rộng.
    // Với mỗi thành phố a thuộc công ty i (nút ảo n+i):
    //   - cạnh a -> (n+i) trọng số c_i : lên xe / mua vé,
    //   - cạnh (n+i) -> a trọng số 0   : xuống xe miễn phí.
    vector<vector<pair<int, long long>>> adj(N + 1);
    for (int i = 1; i <= m; i++) {
        int k = readInt();
        long long c = cost[i];
        int cnode = n + i;
        adj[cnode].reserve(k);
        while (k--) {
            int a = readInt();
            adj[a].push_back({cnode, c});   // lên xe (trả c)
            adj[cnode].push_back({a, 0});    // xuống xe (miễn phí)
        }
    }

    // Dijkstra từ nút 1 (Syrjälä) trên đồ thị trọng số không âm.
    const long long INF = LLONG_MAX;
    vector<long long> dist(N + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> pq;

    dist[1] = 0;
    pq.push({0, 1});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;   // bỏ qua mục cũ đã lỗi thời
        for (auto& e : adj[u]) {
            int v = e.first;
            long long nd = d + e.second;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }

    // In chi phí rẻ nhất tới các thành phố 1..n.
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
