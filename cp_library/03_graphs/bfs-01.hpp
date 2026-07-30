#pragma once
#include <bits/stdc++.h>
using namespace std;
// 0-1 BFS — đường đi ngắn nhất khi trọng số CHỈ là 0 hoặc 1, O(n + m) bằng deque.
// Khi dùng: đồ thị/lưới mà mỗi cạnh tốn 0 hoặc 1 (vd đi thẳng tốn 0, đổi hướng tốn 1).
// ĐPT: O(n + m); bộ nhớ O(n). adj[u] = {(v, w)} với w thuộc {0, 1}.
// Dùng: auto dist = bfs01(adj, s); // dist[v] == -1 nếu không tới
// Bẫy: trọng số PHẢI là 0 hoặc 1; cạnh 0 đẩy front, cạnh 1 đẩy back; nếu trọng số bất kỳ dùng dijkstra.
// CSES: —
// Ý nghĩa: 0-1 BFS tìm khoảng cách ngắn nhất từ s tới mọi đỉnh khi cạnh chỉ nặng 0 hoặc 1.
// Tham số: adj = danh sách kề (0-based), adj[u] = {(v, w)} với w thuộc {0, 1}; s = đỉnh nguồn.
// Trả về: dist[v] = khoảng cách ngắn nhất từ s tới v, hoặc -1 nếu không tới được.
vector<long long> bfs01(const vector<vector<pair<int, int>>>& adj, int s) {
    int n = (int)adj.size();
    vector<long long> dist(n, LLONG_MAX);
    deque<int> dq;
    dist[s] = 0;
    dq.push_front(s);
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : adj[u]) {
            long long nd = dist[u] + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                if (w) dq.push_back(v); else dq.push_front(v);
            }
        }
    }
    for (auto& d : dist) if (d == LLONG_MAX) d = -1;
    return dist;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ bfs-01.hpp -o demo && ./demo
int main() {
    vector<vector<pair<int, int>>> adj = {{{1, 0}, {2, 1}}, {{2, 0}}, {{3, 1}}, {}};
    auto dist = bfs01(adj, 0);
    printf("0-1 BFS: khoảng cách 0 -> 3 = %lld\n", dist[3]);   // 1
    return 0;
}
#endif
