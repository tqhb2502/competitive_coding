#pragma once
#include <bits/stdc++.h>
using namespace std;
// Tree DP (rerooting) — ba mẫu DP trên cây: ghép cặp cực đại, tổng khoảng cách và khoảng cách xa nhất từ MỌI đỉnh.
// Khi dùng: đề hỏi đáp án cho TỪNG đỉnh (tổng/max khoảng cách tới các đỉnh khác) hoặc DP hai trạng thái dp[v][0/1] trên cây.
// ĐPT: mỗi hàm O(n) — một lượt xuống (gom cây con) + một lượt lên (đổi gốc); bộ nhớ O(n). Không sort, không đệ quy.
// Dùng: int m = treeMatching(n, adj);                    // số cạnh của ghép cặp cực đại
// Dùng: auto s = treeRerootSumDist(n, adj); s[v];        // tổng khoảng cách từ v tới mọi đỉnh (long long)
// Dùng: auto f = treeMaxDistAll(n, adj); f[v];           // khoảng cách xa nhất từ v tới một đỉnh khác
// Bẫy: đỉnh 0-based (CSES 1-based → trừ 1, hoặc truyền n+1 và để trống đỉnh 0); tổng khoảng cách phải long long (~n^2/2); cây đơn, không tự khuyên.
// CSES: 1130 1132 1133
// Ý nghĩa: duyệt DFS LẶP (stack tường minh, không tràn stack với n = 2e5) mọi thành phần liên thông.
// Tham số: n = số đỉnh (0-based, adj.size() phải >= n); adj = danh sách kề vô hướng của cây/rừng.
// Trả về: pair (order, par) — order = đủ n đỉnh, cha LUÔN đứng trước con; par[v] = cha của v, -1 nếu v là gốc thành phần.
pair<vector<int>, vector<int>> treeDpOrder(int n, const vector<vector<int>>& adj) {
    vector<int> order, par(n, -1), st;
    order.reserve(n);
    vector<char> vis(n, 0);
    for (int s = 0; s < n; s++) {
        if (vis[s]) continue;
        st.push_back(s); vis[s] = 1;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            order.push_back(u);
            for (int v : adj[u]) if (!vis[v]) { vis[v] = 1; par[v] = u; st.push_back(v); }
        }
    }
    return {order, par};
}

// Ý nghĩa: ghép cặp cực đại trên cây/rừng bằng DP hai trạng thái dp0/dp1, xử lý theo thứ tự đảo của DFS lặp (CSES 1130).
// Tham số: n = số đỉnh (0-based, adj.size() phải >= n); adj = danh sách kề vô hướng của cây/rừng.
// Trả về: số cạnh nhiều nhất chọn được sao cho không hai cạnh nào chung đỉnh (n <= 1 -> 0); rừng -> cộng theo từng thành phần.
int treeMatching(int n, const vector<vector<int>>& adj) {
    auto [order, par] = treeDpOrder(n, adj);
    vector<int> dp0(n, 0), dp1(n, -1);   // dp0[u]: u CHƯA ghép; dp1[u]: u ĐÃ ghép với một con (-1 = không khả thi, u là lá)
    for (int i = n - 1; i >= 0; i--) {   // đảo thứ tự => con luôn xong trước cha
        int u = order[i], best = INT_MIN;
        for (int v : adj[u]) if (v != par[u]) {
            int take = max(dp0[v], dp1[v]);
            dp0[u] += take;                          // con v tự tối ưu
            best = max(best, 1 + dp0[v] - take);     // lợi ích khi đổi sang dùng cạnh u-v
        }
        if (best != INT_MIN) dp1[u] = dp0[u] + best;
    }
    int ans = 0;
    for (int v = 0; v < n; v++) if (par[v] < 0) ans += max(dp0[v], dp1[v]);   // gốc mỗi thành phần
    return ans;
}

// Ý nghĩa: rerooting tính tổng khoảng cách từ mỗi đỉnh tới mọi đỉnh khác, 2 lượt duyệt trên thứ tự DFS lặp (CSES 1133).
// Tham số: n = số đỉnh (0-based, adj.size() phải >= n); adj = danh sách kề vô hướng của cây/rừng, mọi cạnh trọng số 1.
// Trả về: mảng ans (long long) với ans[v] = tổng số cạnh từ v tới mọi đỉnh CÙNG thành phần (cây liên thông -> mọi đỉnh).
vector<long long> treeRerootSumDist(int n, const vector<vector<int>>& adj) {
    auto [order, par] = treeDpOrder(n, adj);
    vector<long long> cnt(n, 1), sub(n, 0), tot(n, 0), ans(n, 0);   // cnt = cỡ cây con, sub = tổng kc trong cây con
    for (int i = n - 1; i >= 0; i--) {                              // lượt xuống: dồn con lên cha
        int u = order[i], p = par[u];
        if (p >= 0) { cnt[p] += cnt[u]; sub[p] += sub[u] + cnt[u]; }
    }
    for (int i = 0; i < n; i++) {   // lượt lên: đổi gốc từ cha p sang con u (cnt[u] đỉnh gần thêm 1, phần còn lại xa thêm 1)
        int u = order[i], p = par[u];
        if (p < 0) { ans[u] = sub[u]; tot[u] = cnt[u]; }
        else { tot[u] = tot[p]; ans[u] = ans[p] + tot[p] - 2 * cnt[u]; }
    }
    return ans;
}

// Ý nghĩa: rerooting tính khoảng cách xa nhất từ mỗi đỉnh, dùng hai giá trị lớn nhất trong các con để loại chính con đó (CSES 1132).
// Tham số: n = số đỉnh (0-based, adj.size() phải >= n); adj = danh sách kề vô hướng của cây/rừng, mọi cạnh trọng số 1.
// Trả về: mảng ans với ans[v] = khoảng cách lớn nhất từ v tới một đỉnh CÙNG thành phần (đỉnh cô lập -> 0).
vector<int> treeMaxDistAll(int n, const vector<vector<int>>& adj) {
    auto [order, par] = treeDpOrder(n, adj);
    vector<int> down(n, 0), up(n, 0), ans(n, 0);   // down = xa nhất trong cây con, up = xa nhất đi qua cha (0 nếu là gốc)
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i], p = par[u];
        if (p >= 0) down[p] = max(down[p], down[u] + 1);
    }
    for (int i = 0; i < n; i++) {
        int u = order[i], b1 = 0, b2 = 0, arg1 = -1;   // b1 >= b2: hai giá trị (1 + down[con]) lớn nhất; 0 = "không có con"
        for (int v : adj[u]) if (v != par[u]) {
            int val = down[v] + 1;
            if (val > b1) { b2 = b1; b1 = val; arg1 = v; } else if (val > b2) b2 = val;
        }
        for (int v : adj[u]) if (v != par[u]) up[v] = 1 + max(up[u], v == arg1 ? b2 : b1);
        ans[u] = max(down[u], up[u]);
    }
    return ans;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ tree-dp.hpp -o demo && ./demo
int main() {
    // Cây 5 đỉnh (gốc 0): cạnh 0-1, 0-2, 1-3, 1-4
    int n = 5;
    vector<vector<int>> adj = {{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
    printf("ghép cặp cực đại = %d\n", treeMatching(n, adj));   // 2 (chọn 0-2 và 1-3)
    auto s = treeRerootSumDist(n, adj);
    printf("tổng khoảng cách:");
    for (int v = 0; v < n; v++) printf(" %lld", s[v]);         // 6 5 9 8 8
    auto f = treeMaxDistAll(n, adj);
    printf("\nkhoảng cách xa nhất:");
    for (int v = 0; v < n; v++) printf(" %d", f[v]);           // 2 2 3 3 3
    printf("\n");
    return 0;
}
#endif
