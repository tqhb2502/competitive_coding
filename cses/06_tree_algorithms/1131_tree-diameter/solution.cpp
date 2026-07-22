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

    const auto farthest_from = [&](const int source) {
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
        return std::pair<int, int>{farthest, distance[farthest]};
    };

    const int endpoint = farthest_from(1).first;
    std::cout << farthest_from(endpoint).second << '\n';
    return 0;
}
