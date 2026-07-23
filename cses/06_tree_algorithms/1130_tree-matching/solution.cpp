#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Danh sách kề của cây (n đỉnh, n - 1 cạnh).
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // Duyệt lặp từ gốc 1 để lấy thứ tự duyệt (order) và mảng parent.
    std::vector<int> parent(n + 1, -1);
    std::vector<int> order;
    order.reserve(n);
    order.push_back(1);
    parent[1] = 0;

    for (std::size_t i = 0; i < order.size(); ++i) {
        const int u = order[i];
        for (const int v : adj[u]) {
            if (v == parent[u]) {
                continue;
            }
            parent[v] = u;
            order.push_back(v);
        }
    }

    // dp0[u]: u KHÔNG được ghép; dp1[u]: u ĐƯỢC ghép với đúng một con.
    const int neg_inf = std::numeric_limits<int>::min() / 4;
    std::vector<int> dp0(n + 1, 0);
    std::vector<int> dp1(n + 1, neg_inf);

    // Xử lý theo thứ tự đảo ngược của order => con tính trước cha (post-order).
    for (int i = n - 1; i >= 0; --i) {
        const int u = order[static_cast<std::size_t>(i)];
        int base = 0;             // tổng max(dp0[c], dp1[c]) trên mọi con c
        int best_extra = neg_inf; // max(1 + dp0[c] - max(dp0[c], dp1[c]))

        for (const int v : adj[u]) {
            if (parent[v] != u) {
                continue;
            }
            const int best_child = std::max(dp0[v], dp1[v]);
            base += best_child;
            // Lợi ích khi đổi con v sang trạng thái ghép cạnh u-v.
            best_extra = std::max(best_extra, 1 + dp0[v] - best_child);
        }

        dp0[u] = base;
        // Chỉ ghép được khi u có con (best_extra hợp lệ); lá giữ neg_inf.
        if (best_extra != neg_inf) {
            dp1[u] = base + best_extra;
        }
    }

    // Đáp án là trạng thái tốt hơn tại gốc.
    std::cout << std::max(dp0[1], dp1[1]) << '\n';
    return 0;
}
