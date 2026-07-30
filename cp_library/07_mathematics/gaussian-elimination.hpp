#pragma once
#include <bits/stdc++.h>
using namespace std;
// Gaussian Elimination (mod nguyên tố) — giải hệ phương trình tuyến tính A x = b trên trường Z_p, O(n·m·min(n,m)).
// Khi dùng: giải/đếm nghiệm hệ tuyến tính theo mod nguyên tố (kể cả GF(2) với p = 2); tìm hạng ma trận.
// ĐPT: O(n·m·min(n,m)); bộ nhớ O(n·m). p PHẢI nguyên tố (dùng nghịch đảo Fermat). Trả trạng thái + một nghiệm.
// Dùng: auto res = gaussMod(A, b, p); // res.status: 0 vô nghiệm, 1 duy nhất, 2 vô số; res.x = một nghiệm
// Bẫy: p nguyên tố; A là n hàng × m cột; biến tự do đặt = 0 trong nghiệm trả về; dùng __int128 khi nhân.
// CSES: 3154
struct GaussResult { int status; vector<long long> x; };   // status: 0 = vô nghiệm, 1 = duy nhất, 2 = vô số
// Ý nghĩa: giải hệ A x = b trên trường Z_p bằng khử Gauss (đưa về dạng bậc thang rút gọn).
// Tham số: A = ma trận hệ số n×m; b = vế phải (dài n); p = modulo NGUYÊN TỐ.
// Trả về: {status, x} với status 0/1/2 và x là một nghiệm (biến tự do = 0).
GaussResult gaussMod(vector<vector<long long>> A, vector<long long> b, long long p) {
    int n = (int)A.size(), m = n ? (int)A[0].size() : 0;
    auto mpow = [&](long long base, long long e) {
        long long r = 1 % p; base %= p; if (base < 0) base += p;
        while (e) { if (e & 1) r = (__int128)r * base % p; base = (__int128)base * base % p; e >>= 1; }
        return r;
    };
    for (int i = 0; i < n; i++) A[i].push_back(((b[i] % p) + p) % p);   // cột tăng cường
    vector<int> where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; col++) {
        int sel = -1;
        for (int i = row; i < n; i++) if (A[i][col] % p) { sel = i; break; }
        if (sel < 0) continue;
        swap(A[sel], A[row]);
        long long inv = mpow(A[row][col], p - 2);
        for (int j = col; j <= m; j++) A[row][j] = (__int128)A[row][j] * inv % p;
        for (int i = 0; i < n; i++)
            if (i != row && A[i][col] % p) {
                long long f = A[i][col];
                for (int j = col; j <= m; j++) A[i][j] = ((A[i][j] - (__int128)f * A[row][j]) % p + p) % p;
            }
        where[col] = row; row++;
    }
    vector<long long> x(m, 0);
    for (int col = 0; col < m; col++) if (where[col] >= 0) x[col] = A[where[col]][m];
    for (int i = 0; i < n; i++) {   // kiểm mâu thuẫn: hàng 0 = giá trị khác 0
        bool allZero = true;
        for (int j = 0; j < m; j++) if (A[i][j] % p) { allZero = false; break; }
        if (allZero && A[i][m] % p) return {0, {}};
    }
    for (int col = 0; col < m; col++) if (where[col] < 0) return {2, x};   // có biến tự do -> vô số nghiệm
    return {1, x};
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ gaussian-elimination.hpp -o demo && ./demo
int main() {
    // x + y = 3 ; x - y = 1  (mod 1e9+7) -> x = 2, y = 1
    vector<vector<long long>> A = {{1, 1}, {1, -1}};
    vector<long long> b = {3, 1};
    auto res = gaussMod(A, b, 1000000007);
    printf("status=%d ; x=%lld y=%lld\n", res.status, res.x[0], res.x[1]);   // 1 ; 2 1
    return 0;
}
#endif
