#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Danh sách kề của đồ thị vô hướng, không trọng số
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // parent[v] = đỉnh liền trước v trên đường đi ngắn nhất; -1 nghĩa là chưa thăm
    std::vector<int> parent(n + 1, -1);
    std::queue<int> bfs;
    parent[1] = 0;  // đánh dấu đỉnh nguồn đã thăm (không có cha thực sự)
    bfs.push(1);

    // BFS từ đỉnh 1: lần đầu chạm tới một đỉnh chính là qua đường đi ngắn nhất
    while (!bfs.empty()) {
        const int u = bfs.front();
        bfs.pop();
        if (u == n) {
            break;  // đã tới đích, dừng sớm
        }
        for (const int v : adj[u]) {
            if (parent[v] == -1) {
                parent[v] = u;
                bfs.push(v);
            }
        }
    }

    // Nếu đỉnh n không được thăm thì không tồn tại đường đi
    if (parent[n] == -1) {
        std::cout << "IMPOSSIBLE\n";
        return 0;
    }

    // Truy vết ngược từ n về 1 qua mảng parent, rồi đảo lại để in theo thứ tự 1 -> n
    std::vector<int> path;
    for (int u = n; u != 0; u = parent[u]) {
        path.push_back(u);
    }
    std::reverse(path.begin(), path.end());

    // In số máy tính trên đường đi và trình tự các máy
    std::cout << path.size() << '\n';
    for (std::size_t i = 0; i < path.size(); ++i) {
        if (i != 0U) {
            std::cout << ' ';
        }
        std::cout << path[i];
    }
    std::cout << '\n';
    return 0;
}
