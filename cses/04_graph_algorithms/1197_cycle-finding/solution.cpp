#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
    int source;
    int destination;
    long long weight;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<Edge> edges(m);
    for (Edge& edge : edges) {
        std::cin >> edge.source >> edge.destination >> edge.weight;
    }

    // Zero at every vertex is equivalent to a zero-weight super-source, so
    // negative cycles in disconnected components are detected as well.
    std::vector<long long> distance(n + 1, 0);
    std::vector<int> parent(n + 1, -1);
    int changed_vertex = -1;

    for (int iteration = 0; iteration < n; ++iteration) {
        changed_vertex = -1;
        for (const Edge& edge : edges) {
            const long long candidate = distance[edge.source] + edge.weight;
            if (candidate < distance[edge.destination]) {
                distance[edge.destination] = candidate;
                parent[edge.destination] = edge.source;
                changed_vertex = edge.destination;
            }
        }
        if (changed_vertex == -1) {
            break;
        }
    }

    if (changed_vertex == -1) {
        std::cout << "NO\n";
        return 0;
    }

    int cycle_vertex = changed_vertex;
    for (int i = 0; i < n; ++i) {
        cycle_vertex = parent[cycle_vertex];
    }

    std::vector<int> cycle;
    int current = cycle_vertex;
    do {
        cycle.push_back(current);
        current = parent[current];
    } while (current != cycle_vertex);
    cycle.push_back(cycle_vertex);
    std::reverse(cycle.begin(), cycle.end());

    std::cout << "YES\n";
    for (int vertex : cycle) {
        std::cout << vertex << ' ';
    }
    std::cout << '\n';
    return 0;
}
