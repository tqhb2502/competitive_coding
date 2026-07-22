#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    using Edge = std::pair<int, long long>;
    std::vector<std::vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long weight;
        std::cin >> a >> b >> weight;
        adj[a].emplace_back(b, weight);
    }

    constexpr long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> distance(n + 1, INF);
    using QueueEntry = std::pair<long long, int>;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>,
                        std::greater<QueueEntry>> heap;

    distance[1] = 0;
    heap.emplace(0, 1);
    while (!heap.empty()) {
        const auto [current_distance, u] = heap.top();
        heap.pop();
        if (current_distance != distance[u]) {
            continue;
        }
        for (const auto [v, weight] : adj[u]) {
            const long long candidate = current_distance + weight;
            if (candidate < distance[v]) {
                distance[v] = candidate;
                heap.emplace(candidate, v);
            }
        }
    }

    for (int u = 1; u <= n; ++u) {
        if (u > 1) {
            std::cout << ' ';
        }
        std::cout << distance[u];
    }
    std::cout << '\n';
    return 0;
}
