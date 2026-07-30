#pragma once
#include <bits/stdc++.h>
using namespace std;
// MST (Kruskal) — cây khung nhỏ nhất của đồ thị VÔ HƯỚNG có trọng số, O(m log m) qua DSU.
// Khi dùng: nối mọi đỉnh với tổng trọng số nhỏ nhất; đồ thị không liên thông -> trả rừng khung nhỏ nhất.
// ĐPT: O(m log m) (sắp cạnh); bộ nhớ O(n + m). Đỉnh 0-based; trọng số long long.
// Dùng: MST g(n); g.addEdge(u, v, w); auto [tong, canh] = g.build(); // canh = danh sách cạnh được chọn
// Bẫy: dùng long long cho tổng; nếu đồ thị rời rạc thì tong là tổng của RỪNG (số cạnh < n-1).
// CSES: 1675 3407 3408 3409
struct MST {
    int n;
    vector<tuple<long long, int, int>> edges;   // (w, u, v)
    // Ý nghĩa: khởi tạo cấu trúc MST cho đồ thị vô hướng có n đỉnh, danh sách cạnh rỗng.
    // Tham số: n = số đỉnh (đỉnh 0-based).
    explicit MST(int n) : n(n) {}
    // Ý nghĩa: thêm một cạnh vô hướng (u, v) trọng số w vào danh sách cạnh.
    // Tham số: u, v = hai đỉnh đầu mút; w = trọng số cạnh (long long).
    void addEdge(int u, int v, long long w) { edges.push_back({w, u, v}); }
    // Ý nghĩa: chạy Kruskal (sắp cạnh tăng dần + DSU) để dựng cây/rừng khung nhỏ nhất.
    // Trả về: cặp {total, used} với total = tổng trọng số, used = danh sách cạnh (u, v) được chọn.
    pair<long long, vector<pair<int, int>>> build() {
        sort(edges.begin(), edges.end());
        vector<int> par(n);
        iota(par.begin(), par.end(), 0);
        function<int(int)> find = [&](int x) {
            while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
            return x;
        };
        long long total = 0;
        vector<pair<int, int>> used;
        for (auto& [w, u, v] : edges) {
            int a = find(u), b = find(v);
            if (a != b) { par[a] = b; total += w; used.push_back({u, v}); }
        }
        return {total, used};
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ mst.hpp -o demo && ./demo
int main() {
    MST g(4);
    g.addEdge(0, 1, 1); g.addEdge(1, 2, 2); g.addEdge(2, 3, 3); g.addEdge(0, 2, 4);
    auto [total, used] = g.build();
    printf("tổng trọng số cây khung nhỏ nhất = %lld (số cạnh %d)\n", total, (int)used.size());   // 6 (3)
    return 0;
}
#endif
