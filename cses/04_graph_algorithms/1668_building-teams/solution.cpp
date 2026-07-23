#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Xây dựng danh sách kề cho đồ thị vô hướng: mỗi cạnh thêm vào cả hai chiều.
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // color = 0 nghĩa là chưa tô màu; màu 1 và màu 2 tương ứng hai đội.
    std::vector<int> color(n + 1, 0);
    std::queue<int> bfs;

    // Duyệt từng thành phần liên thông và tô 2 màu (kiểm tra bipartite) bằng BFS.
    for (int start = 1; start <= n; ++start) {
        if (color[start] != 0) {
            continue;
        }
        color[start] = 1;  // Đỉnh gốc của thành phần: gán đội 1.
        bfs.push(start);

        while (!bfs.empty()) {
            const int u = bfs.front();
            bfs.pop();
            const int opposite = 3 - color[u];  // Màu ngược với u dành cho các đỉnh kề.
            for (const int v : adj[u]) {
                if (color[v] == 0) {
                    // Đỉnh chưa tô: gán màu ngược rồi đưa vào hàng đợi lan truyền.
                    color[v] = opposite;
                    bfs.push(v);
                } else if (color[v] != opposite) {
                    // Cạnh nối hai đỉnh cùng màu -> có chu trình lẻ -> không chia được.
                    std::cout << "IMPOSSIBLE\n";
                    return 0;
                }
            }
        }
    }

    // Tô màu thành công: in đội của từng học sinh theo thứ tự 1..n.
    for (int u = 1; u <= n; ++u) {
        if (u > 1) {
            std::cout << ' ';
        }
        std::cout << color[u];
    }
    std::cout << '\n';
    return 0;
}
