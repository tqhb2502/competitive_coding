#pragma once
#include <bits/stdc++.h>
using namespace std;
// Interval DP — DP trên ĐOẠN [i, j] (mẫu: trò lấy hai đầu, hai người chơi tối ưu), O(n^2).
// Khi dùng: bài toán gộp/tách/chọn trên đoạn liên tục mà lời giải phụ thuộc hai đầu; game lấy đầu-cuối.
// ĐPT: O(n^2); bộ nhớ O(n^2). dp[i][j] = HIỆU điểm (người đi trước - người đi sau) tối ưu trên a[i..j].
// Dùng: long long best = removalGameMaxScore(a); // tổng điểm lớn nhất của người đi trước khi cả hai tối ưu
// Bẫy: đây là MẪU cho trò lấy hai đầu; đổi công thức dp cho bài đoạn khác (gộp đá, đa giác...); dùng long long.
// CSES: 1080 1097 1744
// Ý nghĩa: điểm lớn nhất người đi TRƯỚC đạt được khi hai người lần lượt lấy phần tử ở HAI ĐẦU mảng, cùng tối ưu.
// Tham số: a = mảng giá trị các phần tử.
// Trả về: tổng điểm của người đi trước (theo lối chơi tối ưu của cả hai).
long long removalGameMaxScore(const vector<long long>& a) {
    int n = (int)a.size();
    if (n == 0) return 0;
    vector<vector<long long>> dp(n, vector<long long>(n, 0));   // dp[i][j] = hiệu điểm tối ưu trên [i, j]
    for (int i = 0; i < n; i++) dp[i][i] = a[i];
    for (int len = 2; len <= n; len++)
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            dp[i][j] = max(a[i] - dp[i + 1][j], a[j] - dp[i][j - 1]);
        }
    long long total = accumulate(a.begin(), a.end(), 0LL);
    return (total + dp[0][n - 1]) / 2;   // điểm người trước = (tổng + hiệu)/2
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ interval-dp.hpp -o demo && ./demo
int main() {
    vector<long long> a = {4, 5, 4};
    printf("điểm người đi trước (trò lấy hai đầu) = %lld\n", removalGameMaxScore(a));   // 8
    return 0;
}
#endif
