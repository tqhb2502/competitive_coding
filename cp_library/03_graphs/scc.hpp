#pragma once
#include <bits/stdc++.h>
using namespace std;
// SCC (Tarjan lặp) — thành phần liên thông mạnh của đồ thị CÓ HƯỚNG + rút gọn thành DAG các thành phần.
// Khi dùng: rút gọn chu trình có hướng thành DAG, đếm/gộp thành phần, tiền xử lý 2-SAT.
// ĐPT: build O(n + m), condensation O(n + m log m) (khử trùng bằng set); bộ nhớ O(n + m). comp[v] = id thành phần, tô-pô NGƯỢC (sink id nhỏ).
// Dùng: SCC g(n); g.addEdge(u, v); g.build(); g.comp[v]; g.sccCount; auto dag = g.condensation();
// Bẫy: chỉ cho đồ thị CÓ HƯỚNG; bản LẶP nên không tràn stack; addEdge xong hết mới build().
// CSES: 1682 1683 1685 1686 2143
struct SCC {
    int n, sccCount = 0, timer = 0;
    vector<vector<int>> adj;
    vector<int> comp, num, low, stk;
    vector<char> onStk;
    // Ý nghĩa: khởi tạo đồ thị có hướng n đỉnh (0-based), cấp phát các mảng trạng thái cho Tarjan.
    // Tham số: n = số đỉnh.
    explicit SCC(int n) : n(n), adj(n), comp(n, -1), num(n, -1), low(n, -1), onStk(n, 0) {}
    // Ý nghĩa: thêm cạnh có hướng u -> v vào danh sách kề.
    // Tham số: u = đỉnh đầu; v = đỉnh cuối.
    void addEdge(int u, int v) { adj[u].push_back(v); }
    // Ý nghĩa: chạy Tarjan lặp (không đệ quy) gán comp[v] = id thành phần và đặt sccCount; id theo tô-pô ngược (sink id nhỏ).
    void build() {
        vector<int> callStk, it(n, 0);
        for (int s = 0; s < n; s++) {
            if (num[s] != -1) continue;
            callStk.push_back(s);
            while (!callStk.empty()) {
                int u = callStk.back();
                if (it[u] == 0) { num[u] = low[u] = timer++; stk.push_back(u); onStk[u] = 1; }
                if (it[u] < (int)adj[u].size()) {
                    int v = adj[u][it[u]++];
                    if (num[v] == -1) callStk.push_back(v);
                    else if (onStk[v]) low[u] = min(low[u], num[v]);
                } else {
                    if (low[u] == num[u]) {
                        while (true) {
                            int w = stk.back(); stk.pop_back(); onStk[w] = 0;
                            comp[w] = sccCount;
                            if (w == u) break;
                        }
                        sccCount++;
                    }
                    callStk.pop_back();
                    if (!callStk.empty()) low[callStk.back()] = min(low[callStk.back()], low[u]);
                }
            }
        }
    }
    // Ý nghĩa: dựng đồ thị rút gọn (condensation) trên các thành phần, chỉ giữ cạnh liên thành phần và khử cạnh trùng.
    // Trả về: danh sách kề DAG kích thước sccCount, dag[i] = các thành phần kề sau của thành phần i.
    vector<vector<int>> condensation() {
        vector<vector<int>> dag(sccCount);
        set<pair<int, int>> seen;
        for (int u = 0; u < n; u++)
            for (int v : adj[u])
                if (comp[u] != comp[v] && seen.insert({comp[u], comp[v]}).second)
                    dag[comp[u]].push_back(comp[v]);
        return dag;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ scc.hpp -o demo && ./demo
int main() {
    SCC g(5);
    g.addEdge(0, 1); g.addEdge(1, 2); g.addEdge(2, 0);   // chu trình {0,1,2}
    g.addEdge(2, 3); g.addEdge(3, 4); g.addEdge(4, 3);   // chu trình {3,4}
    g.build();
    printf("số thành phần liên thông mạnh = %d\n", g.sccCount);   // 2
    return 0;
}
#endif
