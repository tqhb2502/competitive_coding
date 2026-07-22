#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

struct Edge {
    int destination;
    long long cost;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<Edge>> graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long cost;
        std::cin >> a >> b >> cost;
        graph[a].push_back({b, cost});
    }

    constexpr int MOD = 1'000'000'007;
    constexpr long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> distance(n + 1, INF);
    std::vector<int> ways(n + 1, 0);
    std::vector<int> minimum_edges(n + 1, 0);
    std::vector<int> maximum_edges(n + 1, 0);

    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> queue;
    distance[1] = 0;
    ways[1] = 1;
    queue.emplace(0, 1);

    while (!queue.empty()) {
        const auto [current_distance, node] = queue.top();
        queue.pop();
        if (current_distance != distance[node]) {
            continue;
        }

        for (const Edge& edge : graph[node]) {
            const long long candidate = current_distance + edge.cost;
            const int candidate_minimum = minimum_edges[node] + 1;
            const int candidate_maximum = maximum_edges[node] + 1;

            if (candidate < distance[edge.destination]) {
                distance[edge.destination] = candidate;
                ways[edge.destination] = ways[node];
                minimum_edges[edge.destination] = candidate_minimum;
                maximum_edges[edge.destination] = candidate_maximum;
                queue.emplace(candidate, edge.destination);
            } else if (candidate == distance[edge.destination]) {
                ways[edge.destination] += ways[node];
                if (ways[edge.destination] >= MOD) {
                    ways[edge.destination] -= MOD;
                }
                minimum_edges[edge.destination] =
                    std::min(minimum_edges[edge.destination], candidate_minimum);
                maximum_edges[edge.destination] =
                    std::max(maximum_edges[edge.destination], candidate_maximum);
            }
        }
    }

    std::cout << distance[n] << ' ' << ways[n] << ' ' << minimum_edges[n] << ' '
              << maximum_edges[n] << '\n';
    return 0;
}
