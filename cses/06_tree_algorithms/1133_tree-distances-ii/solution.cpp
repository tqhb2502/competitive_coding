#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    std::vector<int> parent(n + 1, -1);
    std::vector<int> depth(n + 1, 0);
    std::vector<int> order;
    order.reserve(n);
    order.push_back(1);
    parent[1] = 0;

    for (std::size_t i = 0; i < order.size(); ++i) {
        const int u = order[i];
        for (const int v : adj[u]) {
            if (v == parent[u]) {
                continue;
            }
            parent[v] = u;
            depth[v] = depth[u] + 1;
            order.push_back(v);
        }
    }

    std::vector<int> subtree_size(n + 1, 1);
    for (int i = n - 1; i > 0; --i) {
        const int u = order[static_cast<std::size_t>(i)];
        subtree_size[parent[u]] += subtree_size[u];
    }

    std::vector<long long> answer(n + 1, 0);
    for (int u = 1; u <= n; ++u) {
        answer[1] += depth[u];
    }

    for (std::size_t i = 1; i < order.size(); ++i) {
        const int u = order[i];
        answer[u] = answer[parent[u]] + static_cast<long long>(n) -
                    2LL * subtree_size[u];
    }

    for (int u = 1; u <= n; ++u) {
        if (u > 1) {
            std::cout << ' ';
        }
        std::cout << answer[u];
    }
    std::cout << '\n';
    return 0;
}
