#include <algorithm>
#include <iostream>
#include <limits>
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
            order.push_back(v);
        }
    }

    const int neg_inf = std::numeric_limits<int>::min() / 4;
    std::vector<int> dp0(n + 1, 0);
    std::vector<int> dp1(n + 1, neg_inf);

    for (int i = n - 1; i >= 0; --i) {
        const int u = order[static_cast<std::size_t>(i)];
        int base = 0;
        int best_extra = neg_inf;

        for (const int v : adj[u]) {
            if (parent[v] != u) {
                continue;
            }
            const int best_child = std::max(dp0[v], dp1[v]);
            base += best_child;
            best_extra = std::max(best_extra, 1 + dp0[v] - best_child);
        }

        dp0[u] = base;
        if (best_extra != neg_inf) {
            dp1[u] = base + best_extra;
        }
    }

    std::cout << std::max(dp0[1], dp1[1]) << '\n';
    return 0;
}
