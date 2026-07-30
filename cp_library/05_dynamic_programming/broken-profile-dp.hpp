#pragma once
#include <bits/stdc++.h>
using namespace std;
// Broken profile DP — đếm cách lát kín bảng bằng domino 1x2/2x1, quét từng ô với mặt nạ theo chiều nhỏ.
// Khi dùng: đếm cách lát domino/xếp gạch trên lưới có MỘT chiều nhỏ (min(n, m) <= ~14), có hoặc không ô bị chặn.
// ĐPT: countTilings O(n*m*2^min(n,m)), bộ nhớ O(2^min(n,m)); countTilingsMasked cùng ĐPT (tự chuyển vị),
//      bộ nhớ O(2^min(n,m) + n*m). CSES 2181 (n <= 10, m <= 1000): ~10^7 phép cộng.
// Dùng: countTilings(4, 4) == 36; countTilings(10, 1000, 1000000007);
//       countTilingsMasked({"...", ".#.", "..."}) == 2   // '#' là ô bị chặn, chỉ phủ các ô '.'
// Bẫy: chỉ CỘNG nên mod tùy ý <= ~4.6e18; 2^min(n,m) nổ nếu CẢ HAI chiều lớn; số ô trống lẻ luôn cho 0;
//      trong grid ô '.' là trống, MỌI ký tự khác (thường '#') coi như bị chặn; bảng rỗng tính là 1 cách.
// CSES: 2181

// Ý nghĩa: đếm số cách lát kín bảng n hàng x m cột bằng domino 1x2 và 2x1 (broken profile quét theo hàng,
//          bit thứ k của mặt nạ = ô thứ k kể từ ô đang xét theo thứ tự quét đã bị domino trước phủ chưa).
// Tham số: n = số hàng (>= 0); m = số cột (>= 0); mod = modulo kết quả (1 <= mod <= ~4.6e18 vì chỉ dùng phép cộng).
// Trả về: số cách lát kín theo modulo mod; bảng rỗng (n == 0 hoặc m == 0) trả 1 % mod.
long long countTilings(int n, int m, long long mod = 1000000007) {
    if (n <= 0 || m <= 0) return 1 % mod;
    if (m > n) swap(n, m);   // chuyển vị là song ánh giữa hai tập cách lát -> giữ 2^m nhỏ nhất
    int full = 1 << m;
    vector<long long> dp(full, 0), nxt(full, 0);
    auto addTo = [&](long long& slot, long long v) { slot += v; if (slot >= mod) slot -= mod; };
    dp[0] = 1 % mod;         // trước ô đầu tiên chưa có ô nào bị phủ
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++) {
            fill(nxt.begin(), nxt.end(), 0);
            for (int mask = 0; mask < full; mask++) {
                long long cur = dp[mask];
                if (!cur) continue;
                int shifted = mask >> 1;   // trượt cửa sổ sang ô kế; bit m-1 mới ứng với ô (r+1, c)
                if (mask & 1) {
                    addTo(nxt[shifted], cur);                            // ô (r,c) đã bị phủ sẵn
                } else {
                    if (c + 1 < m && !(mask & 2))                        // domino ngang phủ (r,c),(r,c+1)
                        addTo(nxt[shifted | 1], cur);
                    if (r + 1 < n)                                       // domino dọc phủ (r,c),(r+1,c)
                        addTo(nxt[shifted | (1 << (m - 1))], cur);
                }
            }
            dp.swap(nxt);
        }
    return dp[0];            // không domino nào chìa ra ngoài bảng nên mọi trạng thái sống đều là 0
}

// Ý nghĩa: đếm số cách phủ kín MỌI ô trống của lưới có ô bị chặn bằng domino 1x2 và 2x1 (mỗi domino nằm
//          trong hai ô trống kề nhau); tự chuyển vị lưới để mặt nạ chạy theo chiều nhỏ hơn.
// Tham số: grid = các hàng cùng độ dài, ô '.' là ô trống cần phủ, mọi ký tự khác (thường '#') là ô bị chặn;
//          mod = modulo kết quả (1 <= mod <= ~4.6e18 vì chỉ dùng phép cộng).
// Trả về: số cách phủ kín mọi ô trống theo modulo mod; lưới rỗng trả 1 % mod.
long long countTilingsMasked(const vector<string>& grid, long long mod = 1000000007) {
    int n = (int)grid.size();
    int m = n ? (int)grid[0].size() : 0;
    if (!n || !m) return 1 % mod;
    bool transposed = m > n;                 // xoay để số cột (bề rộng mặt nạ) là chiều nhỏ hơn
    int rows = transposed ? m : n, cols = transposed ? n : m;
    vector<vector<char>> emptyCell(rows, vector<char>(cols, 0));
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++) {
            char ok = (grid[r][c] == '.');
            if (transposed) emptyCell[c][r] = ok; else emptyCell[r][c] = ok;
        }
    int full = 1 << cols;
    vector<long long> dp(full, 0), nxt(full, 0);
    auto addTo = [&](long long& slot, long long v) { slot += v; if (slot >= mod) slot -= mod; };
    dp[0] = 1 % mod;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++) {
            fill(nxt.begin(), nxt.end(), 0);
            for (int mask = 0; mask < full; mask++) {
                long long cur = dp[mask];
                if (!cur) continue;
                int shifted = mask >> 1;
                if ((mask & 1) || !emptyCell[r][c]) {
                    addTo(nxt[shifted], cur);                            // đã bị phủ, hoặc ô bị chặn -> bỏ qua
                } else {
                    if (c + 1 < cols && !(mask & 2) && emptyCell[r][c + 1])
                        addTo(nxt[shifted | 1], cur);                    // domino ngang
                    if (r + 1 < rows && emptyCell[r + 1][c])
                        addTo(nxt[shifted | (1 << (cols - 1))], cur);    // domino dọc
                }
            }
            dp.swap(nxt);
        }
    return dp[0];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ broken-profile-dp.hpp -o demo && ./demo
int main() {
    printf("4x4 = %lld ; 6x6 = %lld ; 3x5 = %lld\n",
           countTilings(4, 4), countTilings(6, 6), countTilings(3, 5));       // 36 ; 6728 ; 0 (15 ô lẻ)
    printf("10x10 (mod lớn) = %lld ; 10x1000 mod 1e9+7 = %lld\n",
           countTilings(10, 10, (long long)1e18), countTilings(10, 1000));    // 258584046368 ; 789797918
    vector<string> holes = {"..#.", "....", ".#..", "...."};                  // 14 ô trống
    printf("lưới 4x4 hai ô chặn = %lld ; 3x3 chặn giữa = %lld\n",
           countTilingsMasked(holes), countTilingsMasked({"...", ".#.", "..."}));   // 4 ; 2 (vòng 8 ô)
    return 0;
}
#endif
