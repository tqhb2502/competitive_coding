#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
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

    constexpr long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<std::array<long long, 2>> distance(
        n + 1, std::array<long long, 2>{INF, INF});

    using State = std::tuple<long long, int, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> queue;
    distance[1][0] = 0;
    queue.emplace(0, 1, 0);

    while (!queue.empty()) {
        const auto [current_distance, node, used_coupon] = queue.top();
        queue.pop();
        if (current_distance != distance[node][used_coupon]) {
            continue;
        }

        for (const Edge& edge : graph[node]) {
            const long long normal_distance = current_distance + edge.cost;
            if (normal_distance < distance[edge.destination][used_coupon]) {
                distance[edge.destination][used_coupon] = normal_distance;
                queue.emplace(normal_distance, edge.destination, used_coupon);
            }

            if (used_coupon == 0) {
                const long long discounted_distance =
                    current_distance + edge.cost / 2;
                if (discounted_distance < distance[edge.destination][1]) {
                    distance[edge.destination][1] = discounted_distance;
                    queue.emplace(discounted_distance, edge.destination, 1);
                }
            }
        }
    }

    std::cout << std::min(distance[n][0], distance[n][1]) << '\n';
    return 0;
}
