#pragma once
#include <bits/stdc++.h>
using namespace std;
// Min-Cost Max-Flow (SPFA / đường tăng rẻ nhất) — đẩy luồng CỰC ĐẠI với TỔNG CHI PHÍ nhỏ nhất.
// Khi dùng: bài toán luồng có chi phí mỗi đơn vị trên cạnh; phân công (assignment); ghép có trọng số.
// ĐPT: O(F * V * E) xấu nhất (F = giá trị luồng); bộ nhớ O(V + E). Đỉnh 0-based; cap, cost là long long.
// Dùng: MCMF g(n); g.addEdge(u, v, cap, cost); auto [flow, cost] = g.run(s, t); // luồng cực đại + chi phí nhỏ nhất
// Bẫy: cho phép cost ÂM trên cạnh (SPFA) nhưng KHÔNG được có chu trình âm; dùng long long; s != t.
// CSES: 2121 2129 2130 2131
struct MCMF {
    struct Edge { int to; long long cap, cost; };
    int n;
    vector<Edge> edges;
    vector<vector<int>> g;
    vector<long long> dist;
    vector<int> pe;      // pe[v] = id cạnh vào v trên đường ngắn nhất
    vector<char> inq;
    // Ý nghĩa: khởi tạo mạng luồng n đỉnh (0-based) với danh sách kề rỗng và các mảng phụ trợ.
    // Tham số: n = số đỉnh của mạng.
    explicit MCMF(int n) : n(n), g(n), dist(n), pe(n), inq(n) {}
    // Ý nghĩa: thêm cạnh có hướng u->v (cap, cost) cùng cạnh ngược cap 0, cost -cost để đẩy luồng ngược.
    // Tham số: u = đỉnh đầu; v = đỉnh cuối; cap = dung lượng; cost = chi phí mỗi đơn vị luồng.
    void addEdge(int u, int v, long long cap, long long cost) {
        g[u].push_back((int)edges.size()); edges.push_back({v, cap, cost});
        g[v].push_back((int)edges.size()); edges.push_back({u, 0, -cost});
    }
    // Ý nghĩa: tìm đường đi chi phí nhỏ nhất từ s tới t theo cạnh còn dung lượng bằng SPFA, lưu dist và pe.
    // Tham số: s = đỉnh nguồn; t = đỉnh đích.
    // Trả về: true nếu tồn tại đường tăng từ s tới t (dist[t] hữu hạn), ngược lại false.
    bool spfa(int s, int t) {
        fill(dist.begin(), dist.end(), LLONG_MAX);
        fill(pe.begin(), pe.end(), -1);
        deque<int> q; dist[s] = 0; q.push_back(s); inq[s] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop_front(); inq[u] = 0;
            for (int id : g[u]) {
                auto& e = edges[id];
                if (e.cap > 0 && dist[u] != LLONG_MAX && dist[u] + e.cost < dist[e.to]) {
                    dist[e.to] = dist[u] + e.cost;
                    pe[e.to] = id;
                    if (!inq[e.to]) { inq[e.to] = 1; q.push_back(e.to); }
                }
            }
        }
        return dist[t] != LLONG_MAX;
    }
    // Ý nghĩa: đẩy luồng cực đại từ s tới t bằng cách lặp lại tìm đường rẻ nhất (SPFA) và bơm luồng dọc đường đó.
    // Tham số: s = đỉnh nguồn; t = đỉnh đích.
    // Trả về: cặp {flow, cost} = giá trị luồng cực đại và tổng chi phí nhỏ nhất tương ứng.
    pair<long long, long long> run(int s, int t) {
        long long flow = 0, cost = 0;
        while (spfa(s, t)) {
            long long f = LLONG_MAX;
            for (int v = t; v != s; ) { int id = pe[v]; f = min(f, edges[id].cap); v = edges[id ^ 1].to; }
            for (int v = t; v != s; ) { int id = pe[v]; edges[id].cap -= f; edges[id ^ 1].cap += f; v = edges[id ^ 1].to; }
            flow += f;
            cost += f * dist[t];
        }
        return {flow, cost};
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ min-cost-max-flow.hpp -o demo && ./demo
int main() {
    MCMF g(4);
    g.addEdge(0, 1, 2, 1); g.addEdge(0, 2, 2, 2);
    g.addEdge(1, 3, 2, 1); g.addEdge(2, 3, 2, 1); g.addEdge(1, 2, 1, 1);
    auto [flow, cost] = g.run(0, 3);
    printf("luồng cực đại = %lld, chi phí nhỏ nhất = %lld\n", flow, cost);   // 4, 10
    return 0;
}
#endif
