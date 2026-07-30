#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Xây dựng danh sách kề adj và mảng bậc vào indegree cho từng đỉnh.
    // Ràng buộc "a trước b" tương ứng với cạnh có hướng a -> b.
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> indegree(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        ++indegree[b];
    }

    // Kahn: khởi tạo hàng đợi với mọi đỉnh có indegree = 0 (không có tiên quyết).
    std::queue<int> ready;
    for (int course = 1; course <= n; ++course) {
        if (indegree[course] == 0) {
            ready.push(course);
        }
    }

    // Lấy dần từng đỉnh ra, giảm bậc vào của các đỉnh kề; đỉnh nào về 0 thì thêm vào.
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

    // Không lấy đủ n đỉnh nghĩa là còn chu trình -> không có thứ tự hợp lệ.
    if (static_cast<int>(order.size()) != n) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    // In ra một thứ tự topo hợp lệ.
    for (int i = 0; i < n; ++i) {
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << order[i];
    }
    std::cout << '\n';
    return 0;
}
