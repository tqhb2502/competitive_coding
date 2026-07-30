#pragma once
#include <bits/stdc++.h>
using namespace std;
// Grid DP — đường đi đơn điệu tránh ô cấm và hình vuông đồng giá trị lớn nhất kết thúc tại mỗi ô.
// Khi dùng: trạng thái phụ thuộc ô trên/trái/chéo; đếm đường đi phải-xuống; DP hình vuông trên lưới.
// ĐPT: O(n*m); bộ nhớ O(m) cho các API bên dưới.
// Dùng: countMonotoneGridPaths(grid, '#', MOD); auto sizes = largestEqualSquareByCell(grid);
// Bẫy: lưới phải chữ nhật; đường đi chỉ xuống/phải; modulo >0; sizes trả ma trận nên tốn O(n*m) output.
// CSES: 1638 3359 3413 3415
// Ý nghĩa: đếm đường từ góc trên-trái tới góc dưới-phải, chỉ đi xuống/phải và không qua ô blocked.
// Tham số: grid = lưới chữ nhật; blocked = ký tự ô cấm; mod = modulo dương.
// Trả về: số đường modulo mod (0 nếu lưới rỗng hoặc đầu/cuối bị chặn).
long long countMonotoneGridPaths(const vector<string>& grid, char blocked = '*',
                                 long long mod = 1000000007LL) {
    if (mod <= 0) throw invalid_argument("countMonotoneGridPaths: mod phải dương");
    if (grid.empty() || grid[0].empty()) return 0;
    int n = (int)grid.size(), m = (int)grid[0].size();
    for (const string& row : grid) if ((int)row.size() != m)
        throw invalid_argument("countMonotoneGridPaths: lưới không chữ nhật");
    vector<long long> dp(m, 0);
    dp[0] = grid[0][0] == blocked ? 0 : 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == blocked) dp[j] = 0;
            else if (j > 0) dp[j] = (dp[j] + dp[j - 1]) % mod;
        }
    }
    return dp[m - 1];
}

// Ý nghĩa: với mỗi ô, tính cạnh lớn nhất của hình vuông toàn cùng ký tự có góc dưới-phải tại ô đó.
// Tham số: grid = lưới chữ nhật.
// Trả về: ma trận size cùng kích thước; size[i][j]>=1 với mọi ô tồn tại.
vector<vector<int>> largestEqualSquareByCell(const vector<string>& grid) {
    if (grid.empty()) return {};
    int n = (int)grid.size(), m = (int)grid[0].size();
    for (const string& row : grid) if ((int)row.size() != m)
        throw invalid_argument("largestEqualSquareByCell: lưới không chữ nhật");
    vector<vector<int>> ans(n, vector<int>(m, 1));
    for (int i = 1; i < n; ++i) for (int j = 1; j < m; ++j) {
        char c = grid[i][j];
        if (grid[i - 1][j] == c && grid[i][j - 1] == c && grid[i - 1][j - 1] == c)
            ans[i][j] = 1 + min({ans[i - 1][j], ans[i][j - 1], ans[i - 1][j - 1]});
    }
    return ans;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ grid-dp.hpp -o demo && ./demo
int main() {
    vector<string> grid = {"...", ".*.", "..."};
    auto square = largestEqualSquareByCell(vector<string>{"aaa", "aaa", "abb"});
    printf("paths=%lld; square@1,2=%d\n", countMonotoneGridPaths(grid), square[1][2]);  // 2, 2
    return 0;
}
#endif
