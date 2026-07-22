#include <iostream>
#include <queue>
#include <vector>

std::vector<char> reachable_from(
    const std::vector<std::vector<int>>& graph,
    const int start
) {
    std::vector<char> visited(graph.size(), false);
    std::queue<int> pending;
    visited[start] = true;
    pending.push(start);

    while (!pending.empty()) {
        const int city = pending.front();
        pending.pop();
        for (const int next : graph[city]) {
            if (!visited[next]) {
                visited[next] = true;
                pending.push(next);
            }
        }
    }
    return visited;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(n + 1);
    std::vector<std::vector<int>> reverse_adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        reverse_adj[b].push_back(a);
    }

    const std::vector<char> forward = reachable_from(adj, 1);
    for (int city = 1; city <= n; ++city) {
        if (!forward[city]) {
            std::cout << "NO\n1 " << city << '\n';
            return 0;
        }
    }

    // Reaching city from 1 in the reversed graph means city reaches 1 in the
    // original graph.
    const std::vector<char> backward = reachable_from(reverse_adj, 1);
    for (int city = 1; city <= n; ++city) {
        if (!backward[city]) {
            std::cout << "NO\n" << city << " 1\n";
            return 0;
        }
    }

    std::cout << "YES\n";
    return 0;
}
