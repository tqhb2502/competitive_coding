#include <cstddef>
#include <iostream>
#include <queue>
#include <vector>

namespace {

struct Edge {
    int destination = 0;
    long long difference = 0;
};

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<Edge>> graph(static_cast<std::size_t>(n) + 1U);
    for (int constraint = 0; constraint < m; ++constraint) {
        int left = 0;
        int right = 0;
        long long sum = 0;
        std::cin >> left >> right >> sum;
        const int from = left - 1;
        graph[static_cast<std::size_t>(from)].push_back({right, sum});
        graph[static_cast<std::size_t>(right)].push_back({from, -sum});
    }

    std::vector<long long> prefix(static_cast<std::size_t>(n) + 1U, 0);
    std::vector<bool> visited(static_cast<std::size_t>(n) + 1U, false);
    std::queue<int> queue;
    bool possible = true;

    for (int start = 0; start <= n && possible; ++start) {
        if (visited[static_cast<std::size_t>(start)]) {
            continue;
        }
        visited[static_cast<std::size_t>(start)] = true;
        prefix[static_cast<std::size_t>(start)] = 0;
        queue.push(start);
        while (!queue.empty() && possible) {
            const int vertex = queue.front();
            queue.pop();
            for (const Edge& edge : graph[static_cast<std::size_t>(vertex)]) {
                const long long required =
                    prefix[static_cast<std::size_t>(vertex)] + edge.difference;
                if (!visited[static_cast<std::size_t>(edge.destination)]) {
                    visited[static_cast<std::size_t>(edge.destination)] = true;
                    prefix[static_cast<std::size_t>(edge.destination)] = required;
                    queue.push(edge.destination);
                } else if (prefix[static_cast<std::size_t>(edge.destination)] != required) {
                    possible = false;
                    break;
                }
            }
        }
    }

    if (!possible) {
        std::cout << "NO\n";
        return 0;
    }

    std::cout << "YES\n";
    for (int i = 1; i <= n; ++i) {
        const long long value = prefix[static_cast<std::size_t>(i)] -
                                prefix[static_cast<std::size_t>(i - 1)];
        std::cout << value << (i == n ? '\n' : ' ');
    }
    return 0;
}
