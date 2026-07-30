#pragma once
#include <bits/stdc++.h>
using namespace std;
// Euler Tour (làm phẳng cây con) — gán mỗi đỉnh khoảng [tin, tout) để đưa truy vấn CÂY CON về truy vấn ĐOẠN.
// Khi dùng: cây con của v là đoạn liên tục [tin[v], tout[v]); ghép với Fenwick/segment-tree cho tổng/cập nhật cây con.
// ĐPT: build O(n); mỗi truy vấn tùy cấu trúc đi kèm; bộ nhớ O(n). Đỉnh 0-based; dựng bằng DFS LẶP (không tràn stack).
// Dùng: EulerTour et(adj, root); // đặt giá trị đỉnh v tại vị trí et.tin[v]; cây con của v = [et.tin[v], et.tout[v])
// Bẫy: đoạn nửa mở [tin, tout); size(v) = tout[v]-tin[v]; u là tổ tiên v <=> tin[u] <= tin[v] < tout[u].
// CSES: 1137 1138 1674
struct EulerTour {
    int n, timer = 0;
    vector<int> tin, tout, par;
    // Ý nghĩa: dựng Euler tour bằng DFS lặp từ root, gán tin/tout/par cho mọi đỉnh (không tràn stack).
    // Tham số: adj = danh sách kề cây vô hướng (0-based); root = đỉnh gốc (mặc định 0).
    EulerTour(const vector<vector<int>>& adj, int root = 0) : n((int)adj.size()), tin(n), tout(n), par(n, -1) {
        vector<int> idx(n, 0), st = {root};
        while (!st.empty()) {
            int u = st.back();
            if (idx[u] == 0) tin[u] = timer++;
            if (idx[u] < (int)adj[u].size()) {
                int v = adj[u][idx[u]++];
                if (v != par[u]) { par[v] = u; st.push_back(v); }
            } else { tout[u] = timer; st.pop_back(); }
        }
    }
    // Ý nghĩa: số đỉnh trong cây con gốc v (kích thước đoạn nửa mở [tin[v], tout[v])).
    // Tham số: v = đỉnh gốc của cây con.
    // Trả về: tout[v] - tin[v].
    int size(int v) const { return tout[v] - tin[v]; }
    // Ý nghĩa: kiểm tra u có phải tổ tiên của v hay không (v nằm trong cây con của u).
    // Tham số: u = đỉnh nghi là tổ tiên; v = đỉnh cần kiểm tra.
    // Trả về: true nếu tin[u] <= tin[v] < tout[u] (u là tổ tiên, gồm cả u == v).
    bool isAncestor(int u, int v) const { return tin[u] <= tin[v] && tin[v] < tout[u]; }   // u là tổ tiên của v?
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ euler-tour.hpp -o demo && ./demo
int main() {
    // Cây (gốc 0): cạnh 0-1, 0-2, 1-3, 1-4
    vector<vector<int>> adj = {{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
    EulerTour et(adj, 0);
    printf("size(1)=%d  isAncestor(1,3)=%d  isAncestor(2,3)=%d\n",
           et.size(1), (int)et.isAncestor(1, 3), (int)et.isAncestor(2, 3));   // 3  1  0
    return 0;
}
#endif
