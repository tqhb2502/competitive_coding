#pragma once
#include <bits/stdc++.h>
using namespace std;
// Edit Distance (Levenshtein) — số phép CHÈN / XÓA / THAY tối thiểu để biến xâu a thành b, O(|a|*|b|).
// Khi dùng: đo độ khác nhau giữa hai xâu; nền cho so khớp mờ, gợi ý sửa lỗi.
// ĐPT: O(n*m); bộ nhớ O(n*m). dp[i][j] = khoảng cách giữa a[0..i) và b[0..j).
// Dùng: int d = editDistance(a, b);
// Bẫy: chi phí mỗi phép = 1; hàng/cột 0 khởi tạo bằng i, j (chèn/xóa toàn bộ); phân biệt hoa/thường.
// CSES: 1639
// Ý nghĩa: tính khoảng cách chỉnh sửa (Levenshtein) giữa hai xâu.
// Tham số: a, b = hai xâu cần so.
// Trả về: số phép chèn/xóa/thay tối thiểu để biến a thành b.
int editDistance(const string& a, const string& b) {
    int n = (int)a.size(), m = (int)b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = (a[i - 1] == b[j - 1]) ? dp[i - 1][j - 1]
                                              : 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
    return dp[n][m];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ edit-distance.hpp -o demo && ./demo
int main() {
    printf("editDistance(\"LOVE\", \"MOVIE\") = %d\n", editDistance("LOVE", "MOVIE"));   // L->M thay, chèn I -> 2
    return 0;
}
#endif
