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
    for (int level = 1; level <= n; ++level) {
        if (indegree[level] == 0) {
            ready.push(level);
        }
    }

    constexpr int MOD = 1'000'000'007;
    std::vector<int> ways(n + 1, 0);
    ways[1] = 1;

    while (!ready.empty()) {
        const int level = ready.front();
        ready.pop();

        for (const int next : adj[level]) {
            ways[next] += ways[level];
            if (ways[next] >= MOD) {
                ways[next] -= MOD;
            }

            --indegree[next];
            if (indegree[next] == 0) {
                ready.push(next);
            }
        }
    }

    std::cout << ways[n] << '\n';
    return 0;
}
