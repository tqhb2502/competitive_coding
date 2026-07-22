#include <algorithm>
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

    std::vector<int> parent(n + 1, -1);
    std::queue<int> bfs;
    parent[1] = 0;
    bfs.push(1);

    while (!bfs.empty()) {
        const int u = bfs.front();
        bfs.pop();
        if (u == n) {
            break;
        }
        for (const int v : adj[u]) {
            if (parent[v] == -1) {
                parent[v] = u;
                bfs.push(v);
            }
        }
    }

    if (parent[n] == -1) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    std::vector<int> path;
    for (int u = n; u != 0; u = parent[u]) {
        path.push_back(u);
    }
    std::reverse(path.begin(), path.end());

    std::cout << path.size() << '\n';
    for (std::size_t i = 0; i < path.size(); ++i) {
        if (i != 0U) {
            std::cout << ' ';
        }
        std::cout << path[i];
    }
    std::cout << '\n';
    return 0;
}
