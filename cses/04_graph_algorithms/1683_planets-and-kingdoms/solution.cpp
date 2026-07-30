#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // adj: đồ thị gốc; reverse_adj: đồ thị đảo chiều mọi cạnh (dùng cho Kosaraju).
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<std::vector<int>> reverse_adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        reverse_adj[b].push_back(a);
    }

    // Bước 1: DFS lặp trên đồ thị gốc, ghi lại thứ tự kết thúc (post-order) vào order.
    // Con trỏ next_edge lưu cạnh kế tiếp cần xét của mỗi đỉnh để tránh tràn stack.
    std::vector<char> visited(n + 1, false);
    std::vector<std::size_t> next_edge(n + 1, 0);
    std::vector<int> order;
    order.reserve(n);

    for (int start = 1; start <= n; ++start) {
        if (visited[start]) {
            continue;
        }
        std::vector<int> stack{start};
        visited[start] = true;

        while (!stack.empty()) {
            const int node = stack.back();
            if (next_edge[node] < adj[node].size()) {
                const int next = adj[node][next_edge[node]++];
                if (!visited[next]) {
                    visited[next] = true;
                    stack.push_back(next);
                }
            } else {
                // Đỉnh đã duyệt hết cạnh: chốt finish order rồi lấy ra khỏi stack.
                order.push_back(node);
                stack.pop_back();
            }
        }
    }

    // Bước 2: duyệt các đỉnh theo thứ tự NGƯỢC của order; mỗi lần DFS trên đồ thị
    // đảo chiều xuất phát từ đỉnh chưa gán nhãn sẽ phủ trọn đúng một SCC (kingdom).
    std::vector<int> component(n + 1, 0);
    int component_count = 0;
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        const int start = *it;
        if (component[start] != 0) {
            continue;
        }

        ++component_count;  // Mở một vương quốc (SCC) mới.
        component[start] = component_count;
        std::vector<int> stack{start};
        while (!stack.empty()) {
            const int node = stack.back();
            stack.pop_back();
            for (const int next : reverse_adj[node]) {
                if (component[next] == 0) {
                    component[next] = component_count;
                    stack.push_back(next);
                }
            }
        }
    }

    // In số vương quốc k rồi in nhãn SCC của từng hành tinh 1..n.
    std::cout << component_count << '\n';
    for (int planet = 1; planet <= n; ++planet) {
        if (planet != 1) {
            std::cout << ' ';
        }
        std::cout << component[planet];
    }
    std::cout << '\n';
    return 0;
}
