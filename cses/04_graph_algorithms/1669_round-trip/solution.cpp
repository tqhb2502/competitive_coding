#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Dựng danh sách kề cho đồ thị VÔ HƯỚNG (mỗi cạnh thêm hai chiều).
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // state: 0 = chưa thăm, 1 = đang nằm trên stack DFS, 2 = đã duyệt xong.
    std::vector<int> state(n + 1, 0);
    // parent[v] = đỉnh mà từ đó ta đi đến v trên cây DFS.
    std::vector<int> parent(n + 1, 0);
    // next_edge[u] = con trỏ duyệt danh sách kề của u cho DFS lặp.
    std::vector<std::size_t> next_edge(n + 1, 0U);
    std::vector<int> cycle;

    // Duyệt từng thành phần liên thông cho tới khi tìm được một chu trình.
    for (int start = 1; start <= n && cycle.empty(); ++start) {
        if (state[start] != 0) {
            continue;
        }

        std::vector<int> stack{start};
        state[start] = 1;
        parent[start] = 0;

        while (!stack.empty() && cycle.empty()) {
            const int u = stack.back();
            // Đã duyệt hết cạnh kề của u -> hoàn tất, lấy u ra khỏi stack.
            if (next_edge[u] == adj[u].size()) {
                state[u] = 2;
                stack.pop_back();
                continue;
            }

            const int v = adj[u][next_edge[u]++];
            // Bỏ qua đúng cạnh quay lại parent (an toàn vì không có cạnh bội).
            if (v == parent[u]) {
                continue;
            }
            if (state[v] == 0) {
                // Cạnh cây: đi sâu xuống v.
                state[v] = 1;
                parent[v] = u;
                stack.push_back(v);
            } else if (state[v] == 1) {
                // Back edge u -> v: v là tổ tiên còn trên stack. Truy ngược
                // theo parent từ u về v để dựng chu trình [u, ..., v, u].
                cycle.push_back(u);
                int current = u;
                while (current != v) {
                    current = parent[current];
                    cycle.push_back(current);
                }
                cycle.push_back(u);
            }
        }
    }

    // Không gặp back edge nào -> đồ thị không có chu trình.
    if (cycle.empty()) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    // In số đỉnh của chu trình rồi tới dãy đỉnh.
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
