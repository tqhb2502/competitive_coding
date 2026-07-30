// Khung đồ thị — đọc danh sách kề rồi DÙNG snippet, không tự cài lại thuật toán.
// (Đừng khai báo lại Edge/dijkstra ở đây: trùng tên với snippet sẽ gây nhập nhằng khi bundle.)
// Biên dịch: g++ -std=c++17 -O2 -Wall -Wextra -I <thư mục gốc repo> graph.cpp -o sol
#include <bits/stdc++.h>
using namespace std;
#include "cp_library/03_graphs/bfs.hpp"                    // không trọng số: bfs / bfsPath
// #include "cp_library/03_graphs/dijkstra.hpp"            // có trọng số >= 0
// #include "cp_library/02_data_structures/dsu.hpp"        // liên thông / Kruskal

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m) || n <= 0) return 0;
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        --a; --b;                                          // đề CSES thường 1-based, snippet dùng 0-based
        adj[a].push_back(b);
        adj[b].push_back(a);                               // bỏ dòng này nếu đồ thị CÓ HƯỚNG
    }
    auto [dist, par] = bfs(adj, 0);
    (void)par;                                             // par dùng cho bfsPath(par, t) khi cần in đường đi
    cout << dist[n - 1] << '\n';                           // TODO: thay bằng yêu cầu thật của đề
    return 0;
}
