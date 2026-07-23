#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Danh sách kề và indegree phục vụ thuật toán Kahn (topo sort).
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> indegree(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        ++indegree[b];
    }

    // Đẩy mọi đỉnh có indegree 0 vào hàng đợi để bắt đầu duyệt topo.
    std::queue<int> ready;
    for (int city = 1; city <= n; ++city) {
        if (indegree[city] == 0) {
            ready.push(city);
        }
    }

    // distance[v] = số thành phố nhiều nhất trên đường đi từ 1 tới v (-1 = chưa tới được).
    // parent[v] = đỉnh liền trước trên đường đi tối ưu, dùng để truy vết.
    std::vector<int> distance(n + 1, -1);
    std::vector<int> parent(n + 1, 0);
    distance[1] = 1;

    // Duyệt các đỉnh theo thứ tự topo, kết hợp DP relax cạnh để tìm longest path.
    while (!ready.empty()) {
        const int city = ready.front();
        ready.pop();

        for (const int next : adj[city]) {
            // Chỉ relax khi city tới được từ đỉnh 1 và cho đường đi dài hơn.
            if (distance[city] != -1 &&
                distance[city] + 1 > distance[next]) {
                distance[next] = distance[city] + 1;
                parent[next] = city;
            }

            // Giảm indegree cho Kahn, về 0 thì đẩy next vào hàng đợi.
            --indegree[next];
            if (indegree[next] == 0) {
                ready.push(next);
            }
        }
    }

    // Không tới được n từ 1 nên không tồn tại đường đi.
    if (distance[n] == -1) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    // Truy vết đường đi: đi ngược theo parent từ n về 1 rồi đảo lại.
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
