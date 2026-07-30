#pragma once
#include <bits/stdc++.h>
using namespace std;
// LCS — dãy con chung DÀI NHẤT của hai xâu (độ dài + dựng lại một dãy), O(|a|*|b|).
// Khi dùng: đo phần chung theo THỨ TỰ (không cần liền kề) giữa hai dãy/xâu; so khớp phiên bản.
// ĐPT: O(n*m); bộ nhớ O(n*m). dp[i][j] = LCS của a[0..i) và b[0..j).
// Dùng: int L = lcsLength(a, b); string s = lcsString(a, b); // s là MỘT dãy con chung dài nhất
// Bẫy: "dãy con" (subsequence) không phải "xâu con" (substring); có thể có nhiều LCS, hàm trả một cái.
// CSES: 3403
// Ý nghĩa: tính độ dài dãy con chung dài nhất của hai xâu.
// Tham số: a, b = hai xâu.
// Trả về: độ dài LCS.
int lcsLength(const string& a, const string& b) {
    int n = (int)a.size(), m = (int)b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = (a[i - 1] == b[j - 1]) ? dp[i - 1][j - 1] + 1 : max(dp[i - 1][j], dp[i][j - 1]);
    return dp[n][m];
}
// Ý nghĩa: dựng lại MỘT dãy con chung dài nhất của hai xâu bằng cách truy vết bảng dp.
// Tham số: a, b = hai xâu.
// Trả về: một xâu là dãy con chung dài nhất (rỗng nếu không có ký tự chung).
string lcsString(const string& a, const string& b) {
    int n = (int)a.size(), m = (int)b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = (a[i - 1] == b[j - 1]) ? dp[i - 1][j - 1] + 1 : max(dp[i - 1][j], dp[i][j - 1]);
    string res;
    for (int i = n, j = m; i > 0 && j > 0; ) {
        if (a[i - 1] == b[j - 1]) { res.push_back(a[i - 1]); i--; j--; }
        else if (dp[i - 1][j] >= dp[i][j - 1]) i--;
        else j--;
    }
    reverse(res.begin(), res.end());
    return res;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ lcs.hpp -o demo && ./demo
int main() {
    string a = "ABCBDAB", b = "BDCAB";
    printf("LCS(\"%s\", \"%s\") độ dài = %d, một dãy = %s\n",
           a.c_str(), b.c_str(), lcsLength(a, b), lcsString(a, b).c_str());   // 4, ví dụ "BCAB"
    return 0;
}
#endif
