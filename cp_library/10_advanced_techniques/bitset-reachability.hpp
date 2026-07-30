#pragma once
#include <bits/stdc++.h>
using namespace std;
// Bitset Reachability — bao đóng bắc cầu trên DAG bằng bitset gói 64-bit, O(n·m/64); đếm/hỏi đỉnh tới được.
// Khi dùng: DAG cần "từ u tới được bao nhiêu đỉnh" hoặc "u có tới được v?" với n <= ~5e4 (đồ thị bất kỳ: nén SCC trước).
// ĐPT: dựng O((n + m)·n/64); reachable O(1); count O(n/64); bộ nhớ n·ceil(n/64)·8 byte ≈ n^2/8 (n=5e4 -> ~312 MB!).
// Dùng: DagReach dr(adj); dr.reachable(u, v); dr.count(u); // count tính CẢ chính u
// Bẫy: chỉ đúng với DAG (có chu trình -> nén bằng scc.hpp rồi chạy trên condensation); bộ nhớ n^2/8 nên n phải nhỏ.
// CSES: 2138 2143
struct DagReach {
    int n, W;
    vector<unsigned long long> b;   // b[v*W .. v*W+W) = bitset các đỉnh tới được từ v
    // Ý nghĩa: dựng bao đóng bắc cầu bằng cách duyệt NGƯỢC thứ tự tô-pô (Kahn) và OR bitset của các đỉnh kề sau.
    // Tham số: adj = danh sách kề của DAG (0-based, adj[u] = các đỉnh v với cạnh u -> v).
    explicit DagReach(const vector<vector<int>>& adj) : n((int)adj.size()), W((n + 63) / 64) {
        b.assign((size_t)n * W, 0ull);
        vector<int> indeg(n, 0), order;
        order.reserve(n);
        for (int u = 0; u < n; u++) for (int v : adj[u]) indeg[v]++;
        queue<int> q;
        for (int u = 0; u < n; u++) if (indeg[u] == 0) q.push(u);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
        }
        for (int i = (int)order.size() - 1; i >= 0; i--) {   // ngược tô-pô: mọi đỉnh kề sau đã xong
            int u = order[i];
            size_t pu = (size_t)u * W;
            b[pu + (u >> 6)] |= 1ull << (u & 63);
            for (int v : adj[u]) {
                size_t pv = (size_t)v * W;
                for (int w = 0; w < W; w++) b[pu + w] |= b[pv + w];
            }
        }
    }
    // Ý nghĩa: kiểm tra có đường đi từ u tới v (u tới chính nó luôn đúng).
    // Tham số: u = đỉnh đầu; v = đỉnh đích.
    // Trả về: true nếu v tới được từ u.
    bool reachable(int u, int v) const { return (b[(size_t)u * W + (v >> 6)] >> (v & 63)) & 1ull; }
    // Ý nghĩa: đếm số đỉnh tới được từ u (kể cả chính u).
    // Tham số: u = đỉnh xuất phát.
    // Trả về: số đỉnh tới được.
    long long count(int u) const {
        long long c = 0;
        for (int w = 0; w < W; w++) c += __builtin_popcountll(b[(size_t)u * W + w]);
        return c;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ bitset-reachability.hpp -o demo && ./demo
int main() {
    // DAG: 0->1, 1->2, 0->3
    vector<vector<int>> adj = {{1, 3}, {2}, {}, {}};
    DagReach dr(adj);
    printf("count(0)=%lld count(1)=%lld ; reachable(0,2)=%d reachable(2,0)=%d\n",
           dr.count(0), dr.count(1), (int)dr.reachable(0, 2), (int)dr.reachable(2, 0));   // 4 2 ; 1 0
    return 0;
}
#endif
