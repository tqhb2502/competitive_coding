#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Cây có n node, n-1 cạnh trọng số 1 -> lưu bằng adjacency list.
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // BFS trả về (mảng khoảng cách từ source, node xa nhất tính từ source).
    // Cạnh trọng số 1 nên BFS cho ra đúng khoảng cách ngắn nhất.
    const auto bfs = [&](const int source) {
        std::vector<int> distance(n + 1, -1);
        std::queue<int> queue;
        distance[source] = 0;
        queue.push(source);
        int farthest = source;

        while (!queue.empty()) {
            const int u = queue.front();
            queue.pop();
            if (distance[u] > distance[farthest]) {
                farthest = u;
            }
            for (const int v : adj[u]) {
                if (distance[v] == -1) {
                    distance[v] = distance[u] + 1;
                    queue.push(v);
                }
            }
        }
        return std::pair<std::vector<int>, int>{std::move(distance), farthest};
    };

    // Bước 1: BFS từ node bất kỳ (node 1) -> A là một đầu mút của diameter.
    const int endpoint_a = bfs(1).second;
    // Bước 2: BFS từ A -> distance_a[] và đầu mút còn lại B của diameter.
    auto [distance_a, endpoint_b] = bfs(endpoint_a);
    // Bước 3: BFS từ B -> distance_b[].
    auto [distance_b, unused] = bfs(endpoint_b);
    (void)unused;

    // Đáp án của mỗi node v = max(distance_a[v], distance_b[v]).
    for (int u = 1; u <= n; ++u) {
        if (u > 1) {
            std::cout << ' ';
        }
        std::cout << std::max(distance_a[u], distance_b[u]);
    }
    std::cout << '\n';
    return 0;
}
