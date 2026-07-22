#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    std::vector<int> color(n + 1, 0);
    std::queue<int> bfs;

    for (int start = 1; start <= n; ++start) {
        if (color[start] != 0) {
            continue;
        }
        color[start] = 1;
        bfs.push(start);

        while (!bfs.empty()) {
            const int u = bfs.front();
            bfs.pop();
            const int opposite = 3 - color[u];
            for (const int v : adj[u]) {
                if (color[v] == 0) {
                    color[v] = opposite;
                    bfs.push(v);
                } else if (color[v] != opposite) {
                    std::cout << "IMPOSSIBLE\n";
                    return 0;
                }
            }
        }
    }

    for (int u = 1; u <= n; ++u) {
        if (u > 1) {
            std::cout << ' ';
        }
        std::cout << color[u];
    }
    std::cout << '\n';
    return 0;
}
