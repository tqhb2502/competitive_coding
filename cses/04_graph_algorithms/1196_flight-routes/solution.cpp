#include <functional>
#include <iostream>
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

    int n, m, k;
    std::cin >> n >> m >> k;

    std::vector<std::vector<Edge>> graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long cost;
        std::cin >> a >> b >> cost;
        graph[a].push_back({b, cost});
    }

    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> queue;
    std::vector<int> finalized_count(n + 1, 0);
    std::vector<long long> answers;
    answers.reserve(k);
    queue.emplace(0, 1);

    while (!queue.empty() && static_cast<int>(answers.size()) < k) {
        const auto [distance, node] = queue.top();
        queue.pop();
        if (finalized_count[node] >= k) {
            continue;
        }

        ++finalized_count[node];
        if (node == n) {
            answers.push_back(distance);
        }

        for (const Edge& edge : graph[node]) {
            if (finalized_count[edge.destination] < k) {
                queue.emplace(distance + edge.cost, edge.destination);
            }
        }
    }

    for (int i = 0; i < static_cast<int>(answers.size()); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << answers[i];
    }
    std::cout << '\n';
    return 0;
}
