#pragma once
#include <bits/stdc++.h>
using namespace std;
// BFS — khoảng cách ngắn nhất (theo SỐ CẠNH) trên đồ thị KHÔNG trọng số + dựng lại đường đi.
// Khi dùng: đường đi ngắn nhất đồ thị không trọng số, khoảng cách theo tầng, thành phần liên thông.
// ĐPT: O(n + m); bộ nhớ O(n). Đỉnh 0-based; adj[u] = danh sách đỉnh kề.
// Dùng: auto [dist, par] = bfs(adj, s); // dist[v]=-1 nếu không tới; bfsPath(par, t) dựng đường đi tới t
// Bẫy: chỉ đúng khi MỌI cạnh cùng trọng số; trọng số 0/1 dùng bfs-01, không âm dùng dijkstra.
// CSES: 1193 1194 1667 1668 1670 1673 1682 1700 1701 1705 1707 1752 2427 3114 3149 3217 3294 3303 3357
// Ý nghĩa: BFS từ nguồn s, tính khoảng cách theo số cạnh và cha để dựng lại đường đi.
// Tham số: adj = danh sách kề đồ thị không trọng số (0-based); s = đỉnh nguồn.
// Trả về: cặp (dist, par); dist[v] = số cạnh ngắn nhất từ s (-1 nếu không tới), par[v] = cha trên cây BFS (-1 nếu không).
pair<vector<int>, vector<int>> bfs(const vector<vector<int>>& adj, int s) {
    int n = (int)adj.size();
    vector<int> dist(n, -1), par(n, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) { dist[v] = dist[u] + 1; par[v] = u; q.push(v); }
    }
    return {dist, par};
}
// Ý nghĩa: dựng lại đường đi từ nguồn tới t bằng mảng cha par (chỉ gọi khi dist[t] != -1).
// Tham số: par = mảng cha do bfs trả về; t = đỉnh đích.
// Trả về: danh sách đỉnh trên đường đi theo thứ tự từ nguồn tới t.
vector<int> bfsPath(const vector<int>& par, int t) {
    vector<int> path;
    for (int v = t; v != -1; v = par[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ bfs.hpp -o demo && ./demo
int main() {
    vector<vector<int>> adj = {{1, 2}, {0, 3}, {0, 3}, {1, 2}};  // 0-1, 0-2, 1-3, 2-3
    auto [dist, par] = bfs(adj, 0);
    printf("khoảng cách tới 3 = %d; đường đi:", dist[3]);   // 2
    for (int v : bfsPath(par, 3)) printf(" %d", v);
    printf("\n");
    return 0;
}
#endif
