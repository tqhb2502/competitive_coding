#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> indegree(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        ++indegree[b];
    }

    std::queue<int> ready;
    for (int course = 1; course <= n; ++course) {
        if (indegree[course] == 0) {
            ready.push(course);
        }
    }

    std::vector<int> order;
    order.reserve(n);
    while (!ready.empty()) {
        const int course = ready.front();
        ready.pop();
        order.push_back(course);

        for (const int next : adj[course]) {
            --indegree[next];
            if (indegree[next] == 0) {
                ready.push(next);
            }
        }
    }

    if (static_cast<int>(order.size()) != n) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << order[i];
    }
    std::cout << '\n';
    return 0;
}
