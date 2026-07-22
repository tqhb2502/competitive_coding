#include <algorithm>
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
    for (int city = 1; city <= n; ++city) {
        if (indegree[city] == 0) {
            ready.push(city);
        }
    }

    // distance[v] is the maximum number of cities on a route from 1 to v.
    std::vector<int> distance(n + 1, -1);
    std::vector<int> parent(n + 1, 0);
    distance[1] = 1;

    while (!ready.empty()) {
        const int city = ready.front();
        ready.pop();

        for (const int next : adj[city]) {
            if (distance[city] != -1 &&
                distance[city] + 1 > distance[next]) {
                distance[next] = distance[city] + 1;
                parent[next] = city;
            }

            --indegree[next];
            if (indegree[next] == 0) {
                ready.push(next);
            }
        }
    }

    if (distance[n] == -1) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    std::vector<int> route;
    for (int city = n; city != 0; city = parent[city]) {
        route.push_back(city);
        if (city == 1) {
            break;
        }
    }
    std::reverse(route.begin(), route.end());

    std::cout << route.size() << '\n';
    for (std::size_t i = 0; i < route.size(); ++i) {
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << route[i];
    }
    std::cout << '\n';
    return 0;
}
