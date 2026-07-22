#include <algorithm>
#include <iostream>
#include <vector>

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

    // First Kosaraju pass: iterative DFS postorder on the original graph.
    std::vector<char> visited(n + 1, false);
    std::vector<std::size_t> next_edge(n + 1, 0);
    std::vector<int> order;
    order.reserve(n);

    for (int start = 1; start <= n; ++start) {
        if (visited[start]) {
            continue;
        }
        std::vector<int> stack{start};
        visited[start] = true;

        while (!stack.empty()) {
            const int node = stack.back();
            if (next_edge[node] < adj[node].size()) {
                const int next = adj[node][next_edge[node]++];
                if (!visited[next]) {
                    visited[next] = true;
                    stack.push_back(next);
                }
            } else {
                order.push_back(node);
                stack.pop_back();
            }
        }
    }

    // Second pass: each traversal in reverse finish order is exactly one SCC.
    std::vector<int> component(n + 1, 0);
    int component_count = 0;
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        const int start = *it;
        if (component[start] != 0) {
            continue;
        }

        ++component_count;
        component[start] = component_count;
        std::vector<int> stack{start};
        while (!stack.empty()) {
            const int node = stack.back();
            stack.pop_back();
            for (const int next : reverse_adj[node]) {
                if (component[next] == 0) {
                    component[next] = component_count;
                    stack.push_back(next);
                }
            }
        }
    }

    std::cout << component_count << '\n';
    for (int planet = 1; planet <= n; ++planet) {
        if (planet != 1) {
            std::cout << ' ';
        }
        std::cout << component[planet];
    }
    std::cout << '\n';
    return 0;
}
