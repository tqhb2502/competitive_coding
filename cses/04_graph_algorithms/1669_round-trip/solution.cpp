#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // 0 = unvisited, 1 = on the current DFS stack, 2 = finished.
    std::vector<int> state(n + 1, 0);
    std::vector<int> parent(n + 1, 0);
    std::vector<std::size_t> next_edge(n + 1, 0U);
    std::vector<int> cycle;

    for (int start = 1; start <= n && cycle.empty(); ++start) {
        if (state[start] != 0) {
            continue;
        }

        std::vector<int> stack{start};
        state[start] = 1;
        parent[start] = 0;

        while (!stack.empty() && cycle.empty()) {
            const int u = stack.back();
            if (next_edge[u] == adj[u].size()) {
                state[u] = 2;
                stack.pop_back();
                continue;
            }

            const int v = adj[u][next_edge[u]++];
            if (v == parent[u]) {
                continue;
            }
            if (state[v] == 0) {
                state[v] = 1;
                parent[v] = u;
                stack.push_back(v);
            } else if (state[v] == 1) {
                // The edge u-v closes the parent path u,...,v into a cycle.
                cycle.push_back(u);
                int current = u;
                while (current != v) {
                    current = parent[current];
                    cycle.push_back(current);
                }
                cycle.push_back(u);
            }
        }
    }

    if (cycle.empty()) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    std::cout << cycle.size() << '\n';
    for (std::size_t i = 0; i < cycle.size(); ++i) {
        if (i != 0U) {
            std::cout << ' ';
        }
        std::cout << cycle[i];
    }
    std::cout << '\n';
    return 0;
}
