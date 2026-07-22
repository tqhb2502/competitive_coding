// Graph Girth - CSES 1707
// https://cses.fi/problemset/task/1707
//
// Tìm girth (độ dài chu trình ngắn nhất) của đồ thị vô hướng không trọng số.
// Nếu không có chu trình nào thì in ra -1.
//
// Cách làm: BFS từ mỗi đỉnh làm gốc. Trong mỗi lần BFS, khi gặp "non-tree edge"
// (u, w) (w đã thăm và w != cha của u) thì cập nhật girth = dist[u]+dist[w]+1.
// Độ phức tạp O(n * (n + m)).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    const int INF = INT_MAX;
    int girth = INF;

    vector<int> dist(n + 1, -1);
    vector<int> par(n + 1, -1);
    vector<int> queueArr(n + 1);

    for (int s = 1; s <= n && girth > 3; ++s) {
        // reset trạng thái cho lần BFS bắt đầu từ s
        for (int v = 1; v <= n; ++v) {
            dist[v] = -1;
            par[v] = -1;
        }

        int head = 0, tail = 0;
        dist[s] = 0;
        par[s] = 0;            // 0 = không có cha (đỉnh 0 không tồn tại)
        queueArr[tail++] = s;

        while (head < tail && girth > 3) {
            int u = queueArr[head++];
            int du = dist[u];
            for (int w : adj[u]) {
                if (dist[w] == -1) {
                    dist[w] = du + 1;
                    par[w] = u;
                    queueArr[tail++] = w;
                } else if (w != par[u]) {
                    // non-tree edge: khép kín một chu trình qua gốc s
                    int cand = du + dist[w] + 1;
                    if (cand < girth) girth = cand;
                }
            }
        }
    }

    if (girth == INF) cout << -1 << '\n';
    else cout << girth << '\n';

    return 0;
}
