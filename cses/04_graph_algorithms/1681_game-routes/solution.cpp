#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Xây danh sách kề (adjacency list) và đếm in-degree cho mỗi đỉnh
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> indegree(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        ++indegree[b];
    }

    // Kahn's algorithm: nạp mọi đỉnh có in-degree = 0 vào hàng đợi ban đầu
    std::queue<int> ready;
    for (int level = 1; level <= n; ++level) {
        if (indegree[level] == 0) {
            ready.push(level);
        }
    }

    // ways: số đường đi từ đỉnh 1 đến từng đỉnh, khởi tạo ways[1] = 1
    constexpr int MOD = 1'000'000'007;
    std::vector<int> ways(n + 1, 0);
    ways[1] = 1;

    // Duyệt theo thứ tự topo, cộng dồn số đường đi sang các đỉnh kế tiếp
    while (!ready.empty()) {
        const int level = ready.front();
        ready.pop();

        for (const int next : adj[level]) {
            // Cộng ways[level] vào ways[next] theo modulo (trừ MOD thay cho phép %)
            ways[next] += ways[level];
            if (ways[next] >= MOD) {
                ways[next] -= MOD;
            }

            // Giảm in-degree; đỉnh về 0 nghĩa là đã sẵn sàng để xử lý
            --indegree[next];
            if (indegree[next] == 0) {
                ready.push(next);
            }
        }
    }

    // Đáp án: số đường đi từ màn 1 đến màn n
    std::cout << ways[n] << '\n';
    return 0;
}
