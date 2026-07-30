#pragma once
#include <bits/stdc++.h>
using namespace std;
// Knuth Optimization — tối ưu DP đoạn dp[i][j] = min(dp[i][m] + dp[m][j]) + cost(i, j) từ O(n^3) xuống O(n^2).
// Khi dùng: bài "cắt/gộp đoạn" mà chi phí thỏa bất đẳng thức tứ giác (vd chi phí cắt = tổng đoạn, gộp đá, Huffman-kiểu).
// ĐPT: O(n^2) nhờ opt[i][j-1] <= opt[i][j] <= opt[i+1][j]; bộ nhớ O(n^2). cost(i, j) là chi phí đoạn NỬA MỞ [i, j).
// Dùng: long long best = knuthDP(n, cost); // cost là hàm/lambda (int i, int j) -> long long
// Bẫy: cost PHẢI thỏa bất đẳng thức tứ giác + đơn điệu theo đoạn, nếu không kết quả SAI; dp đoạn 1 phần tử = 0.
// CSES: 2088
// Ý nghĩa: chi phí nhỏ nhất để tách đoạn [0, n) thành từng phần tử, mỗi lần cắt tốn cost của đoạn đang cắt.
// Tham số: n = số phần tử; cost = hàm chi phí đoạn nửa mở [i, j).
// Trả về: dp[0][n] = tổng chi phí nhỏ nhất (0 nếu n <= 1).
template <class Cost>
long long knuthDP(int n, Cost cost) {
    if (n <= 1) return 0;
    const long long INF = (long long)4e18;
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    vector<vector<int>> opt(n + 1, vector<int>(n + 1, 0));
    for (int i = 0; i + 1 <= n; i++) { dp[i][i + 1] = 0; opt[i][i + 1] = i; }   // đoạn 1 phần tử: không cần cắt
    for (int len = 2; len <= n; len++)
        for (int i = 0; i + len <= n; i++) {
            int j = i + len;
            long long c = cost(i, j);
            dp[i][j] = INF;
            int loP = max(i + 1, opt[i][j - 1]), hiP = min(j - 1, opt[i + 1][j]);
            for (int m = loP; m <= hiP; m++) {
                long long v = dp[i][m] + dp[m][j] + c;
                if (v < dp[i][j]) { dp[i][j] = v; opt[i][j] = m; }
            }
        }
    return dp[0][n];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ knuth-optimization.hpp -o demo && ./demo
int main() {
    vector<long long> a = {1, 2, 3};
    vector<long long> pre(a.size() + 1, 0);
    for (size_t i = 0; i < a.size(); i++) pre[i + 1] = pre[i] + a[i];
    auto cost = [&](int i, int j) { return pre[j] - pre[i]; };   // chi phí một lần cắt = tổng đoạn bị cắt
    printf("chi phí cắt nhỏ nhất của {1,2,3} = %lld\n", knuthDP(3, cost));   // 6 + 3 = 9
    return 0;
}
#endif
