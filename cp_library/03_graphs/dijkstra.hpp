#pragma once
#include <bits/stdc++.h>
using namespace std;
// Dijkstra — đường đi ngắn nhất từ 1 nguồn, trọng số KHÔNG âm, O((n + m) log n).
// Khi dùng: đường đi ngắn nhất có trọng số >= 0 (đồ thị hoặc lưới).
// ĐPT: O((n + m) log n); bộ nhớ O(n). adj[u] = {(v, w)}; DIJ_INF = không tới được.
// Dùng: auto dist = dijkstra(adj, s); // dist[v]==DIJ_INF nếu không tới; par cho đường đi (tùy chọn)
// Bẫy: KHÔNG dùng được với cạnh âm (dùng bellman-ford); mọi khoảng cách hữu hạn cần < DIJ_INF;
//       giá trị >= DIJ_INF trùng sentinel "không tới". Phép cộng cạnh dùng __int128 để không signed-overflow.
// CSES: 1195 1196 1202 1203 1671 2121 2130 2131 3158 3213
const long long DIJ_INF = (long long)4e18;
// Ý nghĩa: tính đường đi ngắn nhất một nguồn trên đồ thị trọng số không âm bằng Dijkstra với heap nhỏ nhất.
// Tham số: adj = danh sách kề 0-based, adj[u] = {(v, w)}; s = đỉnh nguồn; par = (tùy chọn) mảng cha để truy vết đường đi.
// Trả về: mảng dist với dist[v] = khoảng cách ngắn nhất từ s tới v, bằng DIJ_INF nếu không tới được.
vector<long long> dijkstra(const vector<vector<pair<int, long long>>>& adj, int s,
                           vector<int>* par = nullptr) {
    int n = (int)adj.size();
    vector<long long> dist(n, DIJ_INF);
    if (par) par->assign(n, -1);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;  // mục cũ đã bị thay
        for (auto [v, w] : adj[u]) {
            __int128 candidate = (__int128)dist[u] + w;
            if (candidate < dist[v]) {
                dist[v] = (long long)candidate;
                if (par) (*par)[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ dijkstra.hpp -o demo && ./demo
int main() {
    vector<vector<pair<int, long long>>> adj = {{{1, 4}, {2, 1}}, {{3, 1}}, {{1, 2}, {3, 5}}, {}};
    auto dist = dijkstra(adj, 0);
    printf("khoảng cách ngắn nhất 0 -> 3 = %lld\n", dist[3]);   // 4
    return 0;
}
#endif
