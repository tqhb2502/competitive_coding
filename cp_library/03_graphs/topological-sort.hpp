#pragma once
#include <bits/stdc++.h>
using namespace std;
// Topological Sort (Kahn) — thứ tự tô-pô của đồ thị CÓ HƯỚNG; trả về RỖNG nếu có chu trình.
// Khi dùng: xếp thứ tự phụ thuộc, DP trên DAG, kiểm tra đồ thị có chu trình hay không.
// ĐPT: O(n + m); bộ nhớ O(n). Đỉnh 0-based; adj[u] = các đỉnh v với cạnh u->v.
// Dùng: auto ord = topoSort(adj); // ord.size()==n nếu là DAG; ord rỗng nếu tồn tại chu trình
// Bẫy: chỉ dùng cho đồ thị CÓ HƯỚNG; kết quả rỗng đồng nghĩa CÓ chu trình (không phải "không có đỉnh").
// CSES: 1679 1680 1681 1699 1757 2138
// Ý nghĩa: tính thứ tự tô-pô của DAG bằng giải thuật Kahn (khử dần đỉnh bậc vào 0).
// Tham số: adj = danh sách kề đồ thị CÓ HƯỚNG (0-based), adj[u] chứa các v với cạnh u->v.
// Trả về: vector thứ tự tô-pô kích thước n; rỗng nếu đồ thị có chu trình.
vector<int> topoSort(const vector<vector<int>>& adj) {
    int n = (int)adj.size();
    vector<int> indeg(n, 0), ord;
    for (int u = 0; u < n; u++)
        for (int v : adj[u]) indeg[v]++;
    queue<int> q;
    for (int u = 0; u < n; u++)
        if (indeg[u] == 0) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        ord.push_back(u);
        for (int v : adj[u])
            if (--indeg[v] == 0) q.push(v);
    }
    if ((int)ord.size() != n) ord.clear();  // còn đỉnh chưa xử lý -> có chu trình
    return ord;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ topological-sort.hpp -o demo && ./demo
int main() {
    vector<vector<int>> adj = {{1, 2}, {3}, {3}, {}};  // DAG: 0->1, 0->2, 1->3, 2->3
    printf("thứ tự tô-pô:");
    for (int u : topoSort(adj)) printf(" %d", u);       // 0 1 2 3 (hoặc 0 2 1 3)
    printf("\n");
    return 0;
}
#endif
