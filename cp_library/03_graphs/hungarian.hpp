#pragma once
#include <bits/stdc++.h>
using namespace std;
// Hungarian (Kuhn-Munkres) — bài toán phân công: ghép hoàn hảo TRỌNG SỐ NHỎ NHẤT trên ma trận chi phí, O(n^2 * m).
// Khi dùng: gán n hàng cho n (hoặc m >= n) cột phân biệt sao cho tổng chi phí nhỏ nhất (assignment problem).
// ĐPT: O(n^2 * m); bộ nhớ O(n * m). Hàng/cột 0-based; ma trận n x m với n <= m; chi phí long long (cho phép âm).
// Dùng: Hungarian h(n, m); h.set(i, j, cost); auto [tong, gan] = h.solve(); // gan[i] = cột ghép với hàng i
// Bẫy: cần n <= m (nếu n > m thì chuyển vị); tìm MIN (muốn MAX thì đặt cost = -giá trị); mọi ô phải được set.
// CSES: 2129
struct Hungarian {
    int n, m;
    vector<vector<long long>> a;   // 1-indexed nội bộ: a[i+1][j+1]
    // Ý nghĩa: khởi tạo bài toán phân công với ma trận chi phí n x m (1-indexed nội bộ), mọi ô mặc định 0.
    // Tham số: n = số hàng; m = số cột (yêu cầu n <= m).
    Hungarian(int n, int m) : n(n), m(m), a(n + 1, vector<long long>(m + 1, 0)) {}
    // Ý nghĩa: đặt chi phí của ô (hàng i, cột j) trong ma trận (index 0-based bên ngoài).
    // Tham số: i = chỉ số hàng 0-based; j = chỉ số cột 0-based; c = chi phí (cho phép âm).
    void set(int i, int j, long long c) { a[i + 1][j + 1] = c; }   // i, j là 0-based
    // Ý nghĩa: chạy thuật toán Hungarian tìm ghép hoàn hảo trọng số nhỏ nhất trên ma trận chi phí.
    // Trả về: cặp {tong, gan} với tong = tổng chi phí nhỏ nhất; gan[i] = cột (0-based) ghép với hàng i.
    pair<long long, vector<int>> solve() {
        const long long INF = LLONG_MAX / 4;
        vector<long long> u(n + 1, 0), v(m + 1, 0), minv(m + 1);
        vector<int> p(m + 1, 0), way(m + 1, 0);
        vector<char> used(m + 1);
        for (int i = 1; i <= n; i++) {
            p[0] = i;
            int j0 = 0;
            fill(minv.begin(), minv.end(), INF);
            fill(used.begin(), used.end(), false);
            do {
                used[j0] = true;
                int i0 = p[j0], j1 = -1;
                long long delta = INF;
                for (int j = 1; j <= m; j++)
                    if (!used[j]) {
                        long long cur = a[i0][j] - u[i0] - v[j];
                        if (cur < minv[j]) { minv[j] = cur; way[j] = j0; }
                        if (minv[j] < delta) { delta = minv[j]; j1 = j; }
                    }
                if (j1 < 0) break;   // luôn tìm được cột khi n <= m; guard phòng thủ (chặn truy cập ngoài mảng)
                for (int j = 0; j <= m; j++) {
                    if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                    else minv[j] -= delta;
                }
                j0 = j1;
            } while (p[j0] != 0);
            do { int j1 = way[j0]; p[j0] = p[j1]; j0 = j1; } while (j0);
        }
        vector<int> gan(n, -1);
        long long tong = 0;
        for (int j = 1; j <= m; j++)
            if (p[j] >= 1 && p[j] <= n) { gan[p[j] - 1] = j - 1; tong += a[p[j]][j]; }
        return {tong, gan};
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ hungarian.hpp -o demo && ./demo
int main() {
    Hungarian h(3, 3);
    int cost[3][3] = {{4, 1, 3}, {2, 0, 5}, {3, 2, 2}};
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) h.set(i, j, cost[i][j]);
    auto [tong, gan] = h.solve();
    printf("chi phí phân công nhỏ nhất = %lld (hàng 0 -> cột %d)\n", tong, gan[0]);   // 5 (hàng 0 -> cột 1)
    return 0;
}
#endif
