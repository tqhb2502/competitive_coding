#pragma once
#include <bits/stdc++.h>
using namespace std;
// Hopcroft-Karp — ghép đôi CỰC ĐẠI trên đồ thị HAI PHÍA, O(E * sqrt(V)) (nhanh hơn Kuhn khi đồ thị lớn).
// Khi dùng: ghép cực đại giữa hai tập (left/right); nền tảng cho định lý König, phủ đỉnh nhỏ nhất.
// ĐPT: O(E * sqrt(V)); bộ nhớ O(V + E). Left 0-based [0, nl), right 0-based [0, nr); addEdge(u, v) nối chúng.
// Dùng: HopcroftKarp g(nl, nr); g.addEdge(u, v); int M = g.maxMatching(); // matchL[u], matchR[v] = bạn ghép (-1 nếu trống)
// Bẫy: chỉ dùng cho đồ thị HAI PHÍA; u thuộc trái, v thuộc phải; DFS đường tăng có thể sâu O(nl),
//       nên với graph cực sâu trên Windows stack nhỏ hãy đổi dfs sang bản lặp hoặc tăng stack.
// CSES: 1696 1709 3358
struct HopcroftKarp {
    int nl, nr;
    vector<vector<int>> adj;
    vector<int> matchL, matchR, dist;
    int shortestAugmenting = 0;
    // Ý nghĩa: khởi tạo đồ thị hai phía rỗng, chưa ghép đôi (matchL/matchR đều -1).
    // Tham số: nl = số đỉnh trái [0, nl); nr = số đỉnh phải [0, nr).
    HopcroftKarp(int nl, int nr) : nl(nl), nr(nr), adj(nl), matchL(nl, -1), matchR(nr, -1), dist(nl) {}
    // Ý nghĩa: thêm cạnh nối đỉnh trái u với đỉnh phải v vào danh sách kề.
    // Tham số: u = đỉnh trái; v = đỉnh phải.
    void addEdge(int u, int v) { adj[u].push_back(v); }
    // Ý nghĩa: BFS theo tầng gán dist cho các đỉnh trái, tìm xem còn đường tăng luồng không.
    // Trả về: true nếu còn tồn tại đường tăng (còn có thể ghép thêm), false nếu không.
    bool bfs() {
        queue<int> q;
        for (int u = 0; u < nl; u++) {
            if (matchL[u] < 0) { dist[u] = 0; q.push(u); }
            else dist[u] = -1;
        }
        const int INF = numeric_limits<int>::max();
        shortestAugmenting = INF;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] + 1 > shortestAugmenting) continue;
            for (int v : adj[u]) {
                int w = matchR[v];
                if (w < 0) shortestAugmenting = min(shortestAugmenting, dist[u] + 1);
                else if (dist[w] < 0) { dist[w] = dist[u] + 1; q.push(w); }
            }
        }
        return shortestAugmenting != INF;
    }
    // Ý nghĩa: DFS theo tầng dist tìm đường tăng từ đỉnh trái u, nếu thấy thì cập nhật ghép đôi.
    // Tham số: u = đỉnh trái xuất phát.
    // Trả về: true nếu tìm được đường tăng và đã ghép lại, false nếu không.
    bool dfs(int u) {
        for (int v : adj[u]) {
            int w = matchR[v];
            if ((w < 0 && dist[u] + 1 == shortestAugmenting) ||
                (w >= 0 && dist[w] == dist[u] + 1 && dfs(w))) {
                matchL[u] = v; matchR[v] = u; return true;
            }
        }
        dist[u] = -1; return false;
    }
    // Ý nghĩa: chạy các pha BFS + DFS đến khi hết đường tăng để tìm ghép đôi cực đại.
    // Trả về: kích thước ghép đôi cực đại; kết quả nằm trong matchL, matchR.
    int maxMatching() {
        int res = 0;
        for (int v : matchL) res += v != -1;
        while (bfs())
            for (int u = 0; u < nl; u++)
                if (matchL[u] < 0 && dfs(u)) res++;
        return res;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ hopcroft-karp.hpp -o demo && ./demo
int main() {
    HopcroftKarp g(3, 3);
    g.addEdge(0, 0); g.addEdge(0, 1); g.addEdge(1, 0); g.addEdge(2, 2);
    printf("ghép đôi cực đại = %d\n", g.maxMatching());   // 3
    return 0;
}
#endif
