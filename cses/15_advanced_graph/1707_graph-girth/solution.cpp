#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Danh sách kề của đồ thị vô hướng
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    const int INF = INT_MAX;
    int girth = INF;

    vector<int> dist(n + 1, -1);   // khoảng cách BFS; -1 nghĩa là chưa thăm
    vector<int> par(n + 1, -1);    // đỉnh cha trên cây BFS
    vector<int> queueArr(n + 1);   // hàng đợi mảng cho BFS lặp

    // Chạy BFS từ mỗi đỉnh làm gốc; girth nhỏ nhất của đồ thị đơn là 3 nên dừng sớm khi đạt 3
    for (int s = 1; s <= n && girth > 3; ++s) {
        // Reset trạng thái cho lần BFS bắt đầu từ s
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
                    // Cạnh cây: mở rộng cây BFS
                    dist[w] = du + 1;
                    par[w] = u;
                    queueArr[tail++] = w;
                } else if (w != par[u]) {
                    // Non-tree edge: khép kín một chu trình đi qua gốc s
                    int cand = du + dist[w] + 1;
                    if (cand < girth) girth = cand;
                }
            }
        }
    }

    // Không tìm thấy chu trình nào thì in -1
    if (girth == INF) cout << -1 << '\n';
    else cout << girth << '\n';

    return 0;
}
