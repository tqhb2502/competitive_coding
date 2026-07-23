#include <iostream>
#include <queue>
#include <vector>

// BFS trả về mảng đánh dấu những đỉnh tới được từ đỉnh start trên đồ thị graph
std::vector<char> reachable_from(
    const std::vector<std::vector<int>>& graph,
    const int start
) {
    std::vector<char> visited(graph.size(), false);
    std::queue<int> pending;
    visited[start] = true;
    pending.push(start);

    while (!pending.empty()) {
        const int city = pending.front();
        pending.pop();
        for (const int next : graph[city]) {
            if (!visited[next]) {
                visited[next] = true;
                pending.push(next);
            }
        }
    }
    return visited;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Xây danh sách kề cho đồ thị gốc và đồ thị đảo chiều mọi cạnh
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<std::vector<int>> reverse_adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        reverse_adj[b].push_back(a);
    }

    // (1) Từ đỉnh 1 phải tới được mọi đỉnh trong đồ thị gốc,
    //     nếu không thì cặp (1, city) là phản ví dụ
    const std::vector<char> forward = reachable_from(adj, 1);
    for (int city = 1; city <= n; ++city) {
        if (!forward[city]) {
            std::cout << "NO\n1 " << city << '\n';
            return 0;
        }
    }

    // (2) Từ đỉnh 1 phải tới được mọi đỉnh trong đồ thị đảo chiều;
    //     tới được city ở đồ thị đảo nghĩa là city tới được 1 ở đồ thị gốc,
    //     nếu không thì cặp (city, 1) là phản ví dụ
    const std::vector<char> backward = reachable_from(reverse_adj, 1);
    for (int city = 1; city <= n; ++city) {
        if (!backward[city]) {
            std::cout << "NO\n" << city << " 1\n";
            return 0;
        }
    }

    // Cả hai lần BFS đều phủ hết n đỉnh -> đồ thị liên thông mạnh
    std::cout << "YES\n";
    return 0;
}
