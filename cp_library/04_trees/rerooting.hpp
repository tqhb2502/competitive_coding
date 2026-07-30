#pragma once
#include <bits/stdc++.h>
using namespace std;
// Rerooting (DP đổi gốc) — tính đáp án cho MỌI đỉnh làm gốc trong O(n); mẫu: tổng khoảng cách từ v tới mọi đỉnh.
// Khi dùng: cần đại lượng phụ thuộc "toàn cây nhìn từ v" cho mọi v (tổng/khoảng cách xa nhất/đếm...) trên CÂY.
// ĐPT: O(n) (một lượt xuống gom cây con + một lượt lên đổi gốc); bộ nhớ O(n). Đỉnh 0-based; cây không trọng số.
// Dùng: RerootDist r(adj); r.ans[v]; // tổng số cạnh từ v tới tất cả các đỉnh khác
// Bẫy: đây là MẪU cho tổng khoảng cách; đổi công thức gộp (down/up) để giải bài rerooting khác; dùng long long.
// CSES: 1133
struct RerootDist {
    int n;
    vector<long long> ans;
    // Ý nghĩa: chạy rerooting bằng 2 lượt BFS-order để tính ans[v] = tổng khoảng cách từ v tới mọi đỉnh.
    // Tham số: adj = danh sách kề cây vô hướng không trọng số (0-based); root = đỉnh gốc khởi đầu (mặc định 0).
    RerootDist(const vector<vector<int>>& adj, int root = 0) : n((int)adj.size()) {
        vector<long long> sub(n, 0), cnt(n, 1);
        vector<int> par(n, -1), order;
        order.reserve(n);
        vector<char> vis(n, 0);
        queue<int> q; q.push(root); vis[root] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop(); order.push_back(u);
            for (int v : adj[u]) if (!vis[v]) { vis[v] = 1; par[v] = u; q.push(v); }
        }
        for (int i = n - 1; i >= 0; i--) {   // lượt xuống (post-order): cnt = cỡ cây con, sub = tổng kc trong cây con
            int u = order[i];
            for (int v : adj[u]) if (v != par[u]) { cnt[u] += cnt[v]; sub[u] += sub[v] + cnt[v]; }
        }
        ans.assign(n, 0);
        ans[root] = sub[root];
        for (int i = 0; i < n; i++) {         // lượt lên (pre-order): đổi gốc từ u xuống con v
            int u = order[i];
            for (int v : adj[u]) if (v != par[u]) ans[v] = ans[u] - cnt[v] + (n - cnt[v]);
        }
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ rerooting.hpp -o demo && ./demo
int main() {
    // Cây (gốc 0): cạnh 0-1, 0-2, 1-3, 1-4
    vector<vector<int>> adj = {{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
    RerootDist r(adj, 0);
    printf("tổng khoảng cách: từ 0 = %lld, từ 1 = %lld, từ 3 = %lld\n",
           r.ans[0], r.ans[1], r.ans[3]);   // 0 -> 6, 1 -> 5, 3 -> 8
    return 0;
}
#endif
