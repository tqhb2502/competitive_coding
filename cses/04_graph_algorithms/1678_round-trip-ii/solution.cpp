#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Danh sách kề: mỗi chuyến bay a -> b là một cạnh có hướng.
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
    }

    // Tô màu 3 trạng thái: 0 = white (chưa thăm), 1 = gray (đang trên stack DFS
    // hiện tại), 2 = black (đã xử lý xong).
    std::vector<int> color(n + 1, 0);
    std::vector<int> parent(n + 1, 0);
    std::vector<std::size_t> next_edge(n + 1, 0U);  // con trỏ chỉ số trong adj[u]
    std::vector<int> cycle;

    // Chạy DFS từ mọi đỉnh chưa thăm để bao phủ cả đồ thị không liên thông.
    for (int start = 1; start <= n && cycle.empty(); ++start) {
        if (color[start] != 0) {
            continue;
        }

        // DFS iterative với stack tường minh (tránh tràn stack khi n lớn).
        std::vector<int> stack{start};
        color[start] = 1;
        parent[start] = 0;

        while (!stack.empty() && cycle.empty()) {
            const int u = stack.back();
            // Đã duyệt hết cạnh của u: tô black rồi pop khỏi stack.
            if (next_edge[u] == adj[u].size()) {
                color[u] = 2;
                stack.pop_back();
                continue;
            }

            const int v = adj[u][next_edge[u]++];
            if (color[v] == 0) {
                // Đỉnh white: đi tiếp, ghi cha và push vào stack.
                color[v] = 1;
                parent[v] = u;
                stack.push_back(v);
            } else if (color[v] == 1) {
                // Back edge u -> v: tìm được chu trình v -> ... -> u -> v.
                // Truy vết ngược theo parent từ u lên tới v rồi đảo lại.
                cycle.push_back(v);
                for (int current = u; current != v; current = parent[current]) {
                    cycle.push_back(current);
                }
                cycle.push_back(v);
                std::reverse(cycle.begin(), cycle.end());
            }
            // color[v] == 2 (black): bỏ qua, không tạo chu trình mới.
        }
    }

    // Không có chu trình nào.
    if (cycle.empty()) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    // In số đỉnh (đỉnh đầu và cuối trùng nhau) rồi in dãy đỉnh của chu trình.
    std::cout << cycle.size() << '\n';
    for (std::size_t i = 0; i < cycle.size(); ++i) {
        if (i != 0U) {
            std::cout << ' ';
        }
        std::cout << cycle[i];
    }
    std::cout << '\n';
    return 0;
}
