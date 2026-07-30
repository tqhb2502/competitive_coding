#pragma once
#include <bits/stdc++.h>
using namespace std;
// Small-to-Large (DSU on tree) — gộp tập dữ liệu cây con theo kiểu "nhỏ nhập vào lớn", O(n log^2 n).
// Khi dùng: truy vấn tổng hợp trên MỌI cây con (đếm màu phân biệt, tần suất...) khi giữ được tập của con lớn nhất.
// ĐPT: O(n log^2 n) (mỗi phần tử bị dời O(log n) lần × O(log n) thao tác set); bộ nhớ O(n). Đỉnh 0-based.
// Dùng: auto ans = distinctColors(adj, color, root); // ans[v] = số màu phân biệt trong cây con của v
// Bẫy: đây là MẪU đếm màu phân biệt; đổi cấu trúc (set -> map tần suất...) cho bài khác; DFS LẶP (không tràn stack).
// CSES: 1139
// Ý nghĩa: với mỗi đỉnh, đếm số màu phân biệt trong cây con của nó bằng kỹ thuật small-to-large.
// Tham số: adj = danh sách kề cây vô hướng (0-based); color = màu của mỗi đỉnh; root = gốc (mặc định 0).
// Trả về: mảng ans với ans[v] = số màu phân biệt trong cây con gốc v.
vector<long long> distinctColors(const vector<vector<int>>& adj, const vector<int>& color, int root = 0) {
    int n = (int)adj.size();
    vector<long long> ans(n, 0);
    vector<int> par(n, -1), order;
    vector<char> vis(n, 0);
    vector<int> st = {root}; vis[root] = 1;
    while (!st.empty()) {   // thứ tự DFS (lặp); xử lý đảo lại = con trước cha
        int u = st.back(); st.pop_back(); order.push_back(u);
        for (int v : adj[u]) if (!vis[v]) { vis[v] = 1; par[v] = u; st.push_back(v); }
    }
    vector<set<int>> S(n);
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i], big = -1;
        for (int v : adj[u]) if (v != par[u]) if (big == -1 || S[v].size() > S[big].size()) big = v;   // con có tập lớn nhất
        if (big != -1) S[u].swap(S[big]);   // giữ tập con lớn nhất, khỏi chép
        for (int v : adj[u]) if (v != par[u] && v != big) { for (int c : S[v]) S[u].insert(c); S[v].clear(); }
        S[u].insert(color[u]);
        ans[u] = (long long)S[u].size();
    }
    return ans;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ small-to-large.hpp -o demo && ./demo
int main() {
    // Cây (gốc 0): cạnh 0-1, 0-2, 1-3, 1-4 ; màu = [1, 2, 1, 2, 3]
    vector<vector<int>> adj = {{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
    vector<int> color = {1, 2, 1, 2, 3};
    auto ans = distinctColors(adj, color, 0);
    printf("số màu phân biệt: cây con 0 = %lld, cây con 1 = %lld\n", ans[0], ans[1]);   // 0 -> {1,2,3}=3, 1 -> {2,3}=2
    return 0;
}
#endif
