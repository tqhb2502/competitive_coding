#pragma once
#include <bits/stdc++.h>
using namespace std;
// Bitmask DP — DP trên TẬP CON (mask): TSP chi phí nhỏ nhất và đếm đường Hamilton, O(2^n * n^2), n <= ~20.
// Khi dùng: n nhỏ (<= 20) cần duyệt mọi tập đỉnh đã thăm; TSP, đếm/đường Hamilton, phủ tập con.
// ĐPT: O(2^n * n^2); bộ nhớ O(2^n * n). Đỉnh 0-based; dp[mask][v] = trạng thái "đã thăm mask, đang ở v".
// Dùng: tspMinCost(dist); countHamiltonianPaths(n, adj, s, t);
// Bẫy: n <= ~20 (2^n bùng nổ); TSP cần dist[i][i]=0; đếm đường Hamilton lấy theo MOD; dùng long long.
// CSES: 1653 1690
// Ý nghĩa: chi phí chu trình Hamilton nhỏ nhất (TSP) xuất phát và kết thúc tại đỉnh 0, đi qua mọi đỉnh đúng một lần.
// Tham số: dist = ma trận chi phí n x n (dist[i][j] = chi phí đi i -> j).
// Trả về: tổng chi phí nhỏ nhất của chu trình đi hết n đỉnh rồi về 0.
long long tspMinCost(const vector<vector<long long>>& dist) {
    int n = (int)dist.size();
    const long long INF = (long long)4e18;
    vector<vector<long long>> dp(1 << n, vector<long long>(n, INF));
    dp[1][0] = 0;
    for (int mask = 1; mask < (1 << n); mask++)
        for (int v = 0; v < n; v++) if ((mask >> v & 1) && dp[mask][v] < INF)
            for (int u = 0; u < n; u++) if (!(mask >> u & 1))
                dp[mask | 1 << u][u] = min(dp[mask | 1 << u][u], dp[mask][v] + dist[v][u]);
    long long best = INF;
    int full = (1 << n) - 1;
    for (int v = 0; v < n; v++) best = min(best, dp[full][v] + dist[v][0]);
    return best;
}
// Ý nghĩa: đếm số đường Hamilton (đi qua MỌI đỉnh đúng một lần) từ s tới t trên đồ thị có hướng.
// Tham số: n = số đỉnh; adj = ma trận kề (adj[v][u] != 0 nếu có cạnh v -> u); s, t = đỉnh đầu/cuối; MOD = số dư.
// Trả về: số đường Hamilton từ s tới t theo modulo MOD.
long long countHamiltonianPaths(int n, const vector<vector<char>>& adj, int s, int t, long long MOD = 1000000007) {
    vector<vector<long long>> dp(1 << n, vector<long long>(n, 0));
    dp[1 << s][s] = 1;
    for (int mask = 0; mask < (1 << n); mask++)
        for (int v = 0; v < n; v++) if ((mask >> v & 1) && dp[mask][v])
            for (int u = 0; u < n; u++) if (!(mask >> u & 1) && adj[v][u])
                dp[mask | 1 << u][u] = (dp[mask | 1 << u][u] + dp[mask][v]) % MOD;
    return dp[(1 << n) - 1][t];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ bitmask-dp.hpp -o demo && ./demo
int main() {
    vector<vector<long long>> dist = {{0, 10, 15, 20}, {10, 0, 35, 25}, {15, 35, 0, 30}, {20, 25, 30, 0}};
    vector<vector<char>> adj = {{0, 1, 0}, {0, 0, 1}, {0, 0, 0}};   // 0->1, 1->2
    printf("TSP min = %lld ; số đường Hamilton 0->2 = %lld\n",
           tspMinCost(dist), countHamiltonianPaths(3, adj, 0, 2));   // 80 (0-1-3-2-0) ; 1 (0-1-2)
    return 0;
}
#endif
