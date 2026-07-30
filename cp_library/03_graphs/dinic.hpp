#pragma once
#include <bits/stdc++.h>
using namespace std;
// Dinic — luồng cực đại trên đồ thị CÓ HƯỚNG, O(V^2 * E) (nhanh hơn nhiều thực tế; O(E*sqrt(V)) khi cap = 1).
// Khi dùng: max-flow / min-cut, ghép đôi hai phía, số đường đi cạnh-rời, bài toán mô hình hoá thành luồng.
// ĐPT: O(V^2 * E); bộ nhớ O(V + E). Đỉnh 0-based; cap là long long; addEdge có hướng (dùng cap hai chiều nếu cần).
// Dùng: Dinic g(n); g.addEdge(u, v, cap); long long F = g.maxflow(s, t); // sau đó cap còn lại = luồng dư
// Bẫy: cap phải >= 0; cạnh vô hướng -> addEdge(u,v,c) rồi addEdge(v,u,c); s != t.
//       dfs có thể sâu O(V); trên Windows stack thường nhỏ, graph dạng đường rất sâu nên dùng bản lặp/tăng stack.
// CSES: 1694 1695 1711 2432
struct Dinic {
    struct Edge { int to; long long cap; };
    int n;
    vector<Edge> edges;
    vector<vector<int>> g;
    vector<int> level, it;
    // Ý nghĩa: khởi tạo mạng luồng rỗng với n đỉnh (0-based), cấp phát danh sách kề và mảng phụ.
    // Tham số: n = số đỉnh.
    explicit Dinic(int n) : n(n), g(n), level(n), it(n) {}
    // Ý nghĩa: thêm cạnh có hướng u -> v với sức chứa cap, kèm cạnh ngược sức chứa 0 để lưu luồng dư.
    // Tham số: u = đỉnh đầu; v = đỉnh cuối; cap = sức chứa cạnh.
    void addEdge(int u, int v, long long cap) {
        if (u < 0 || u >= n || v < 0 || v >= n)
            throw out_of_range("Dinic::addEdge: đỉnh ngoài miền");
        if (cap < 0) throw invalid_argument("Dinic::addEdge: sức chứa âm");
        g[u].push_back((int)edges.size()); edges.push_back({v, cap});
        g[v].push_back((int)edges.size()); edges.push_back({u, 0});
    }
    // Ý nghĩa: BFS trên các cạnh còn dư để dựng đồ thị phân tầng (level) từ nguồn s.
    // Tham số: s = đỉnh nguồn; t = đỉnh đích.
    // Trả về: true nếu còn đường tăng luồng tới t (level[t] >= 0), ngược lại false.
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q; level[s] = 0; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : g[u]) {
                auto& e = edges[id];
                if (e.cap > 0 && level[e.to] < 0) { level[e.to] = level[u] + 1; q.push(e.to); }
            }
        }
        return level[t] >= 0;
    }
    // Ý nghĩa: DFS đẩy luồng theo đồ thị phân tầng, dùng con trỏ it[u] để bỏ qua cạnh đã cạn.
    // Tham số: u = đỉnh hiện tại; t = đỉnh đích; f = luồng tối đa có thể đẩy tới u.
    // Trả về: lượng luồng thực sự đẩy được từ u tới t (0 nếu không có đường).
    long long dfs(int u, int t, long long f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); i++) {
            int id = g[u][i];
            auto& e = edges[id];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                long long d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) { e.cap -= d; edges[id ^ 1].cap += d; return d; }
            }
        }
        return 0;
    }
    // Ý nghĩa: tính luồng cực đại từ s tới t bằng lặp dựng tầng (bfs) rồi đẩy luồng chặn (dfs).
    // Tham số: s = đỉnh nguồn; t = đỉnh đích.
    // Trả về: tổng luồng cực đại; sau đó cap còn lại trên mỗi cạnh là sức chứa dư.
    // Ném overflow_error nếu tổng luồng không vừa long long.
    long long maxflow(int s, int t) {
        if (s < 0 || s >= n || t < 0 || t >= n)
            throw out_of_range("Dinic::maxflow: đỉnh ngoài miền");
        if (s == t) throw invalid_argument("Dinic::maxflow: cần s != t");
        long long flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (long long f = dfs(s, t, LLONG_MAX)) {
                if (flow > LLONG_MAX - f)
                    throw overflow_error("Dinic::maxflow: tổng luồng vượt long long");
                flow += f;
            }
        }
        return flow;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ dinic.hpp -o demo && ./demo
int main() {
    Dinic g(4);
    g.addEdge(0, 1, 3); g.addEdge(0, 2, 2); g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2); g.addEdge(2, 3, 3);
    printf("luồng cực đại 0 -> 3 = %lld\n", g.maxflow(0, 3));   // 5
    return 0;
}
#endif
