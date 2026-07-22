#include <iostream>
#include <limits>
#include <queue>
#include <vector>

struct Edge {
    int from;
    int to;
    long long score;
};

std::vector<char> reachable_from(int start,
                                 const std::vector<std::vector<int>>& adj) {
    std::vector<char> seen(adj.size(), false);
    std::queue<int> bfs;
    seen[start] = true;
    bfs.push(start);
    while (!bfs.empty()) {
        const int u = bfs.front();
        bfs.pop();
        for (const int v : adj[u]) {
            if (!seen[v]) {
                seen[v] = true;
                bfs.push(v);
            }
        }
    }
    return seen;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<Edge> all_edges;
    all_edges.reserve(static_cast<std::size_t>(m));
    std::vector<std::vector<int>> forward(n + 1), reverse(n + 1);
    for (int i = 0; i < m; ++i) {
        Edge edge{};
        std::cin >> edge.from >> edge.to >> edge.score;
        all_edges.push_back(edge);
        forward[edge.from].push_back(edge.to);
        reverse[edge.to].push_back(edge.from);
    }

    const std::vector<char> from_start = reachable_from(1, forward);
    const std::vector<char> to_target = reachable_from(n, reverse);
    std::vector<char> relevant(n + 1, false);
    int relevant_count = 0;
    for (int u = 1; u <= n; ++u) {
        if (from_start[u] && to_target[u]) {
            relevant[u] = true;
            ++relevant_count;
        }
    }

    std::vector<Edge> edges;
    edges.reserve(all_edges.size());
    for (const Edge& edge : all_edges) {
        if (relevant[edge.from] && relevant[edge.to]) {
            edges.push_back(edge);
        }
    }

    constexpr long long NEG_INF = std::numeric_limits<long long>::lowest() / 4;
    std::vector<long long> distance(n + 1, NEG_INF);
    distance[1] = 0;

    for (int round = 1; round < relevant_count; ++round) {
        bool changed = false;
        for (const Edge& edge : edges) {
            if (distance[edge.from] == NEG_INF) {
                continue;
            }
            const long long candidate = distance[edge.from] + edge.score;
            if (candidate > distance[edge.to]) {
                distance[edge.to] = candidate;
                changed = true;
            }
        }
        if (!changed) {
            break;
        }
    }

    for (const Edge& edge : edges) {
        if (distance[edge.from] != NEG_INF &&
            distance[edge.from] + edge.score > distance[edge.to]) {
            std::cout << -1 << '\n';
            return 0;
        }
    }

    std::cout << distance[n] << '\n';
    return 0;
}
