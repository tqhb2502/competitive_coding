#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    const auto bfs = [&](const int source) {
        std::vector<int> distance(n + 1, -1);
        std::queue<int> queue;
        distance[source] = 0;
        queue.push(source);
        int farthest = source;

        while (!queue.empty()) {
            const int u = queue.front();
            queue.pop();
            if (distance[u] > distance[farthest]) {
                farthest = u;
            }
            for (const int v : adj[u]) {
                if (distance[v] == -1) {
                    distance[v] = distance[u] + 1;
                    queue.push(v);
                }
            }
        }
        return std::pair<std::vector<int>, int>{std::move(distance), farthest};
    };

    const int endpoint_a = bfs(1).second;
    auto [distance_a, endpoint_b] = bfs(endpoint_a);
    auto [distance_b, unused] = bfs(endpoint_b);
    (void)unused;

    for (int u = 1; u <= n; ++u) {
        if (u > 1) {
            std::cout << ' ';
        }
        std::cout << std::max(distance_a[u], distance_b[u]);
    }
    std::cout << '\n';
    return 0;
}
