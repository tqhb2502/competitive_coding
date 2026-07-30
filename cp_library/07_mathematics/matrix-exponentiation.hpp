#pragma once
#include <bits/stdc++.h>
using namespace std;
// Matrix Exponentiation — nhân và lũy thừa ma trận theo mod, O(n^3 log e); giải hồi quy tuyến tính, đếm đường độ dài k.
// Khi dùng: số Fibonacci/hồi quy tuyến tính bậc k; đếm số đường đi đúng k cạnh trên đồ thị; k rất lớn.
// ĐPT: nhân O(n^3), pow O(n^3 log e); bộ nhớ O(n^2). Ma trận vuông n x n, phần tử theo mod (long long + __int128).
// Dùng: Matrix M(n, MOD); M.a[i][j] = ...; Matrix R = M.pow(e); // R = M^e theo mod
// Bẫy: đặt mod trước khi nạp phần tử; identity theo mod (1 % mod); dùng __int128 khi nhân; e >= 0.
// CSES: 1096 1722 1723 1726 2413 3169
struct Matrix {
    int n;
    long long mod;
    vector<vector<long long>> a;
    // Ý nghĩa: tạo ma trận vuông n x n toàn 0 với modulo cho trước.
    // Tham số: n = cỡ ma trận; mod = modulo.
    Matrix(int n, long long mod) : n(n), mod(mod), a(n, vector<long long>(n, 0)) {}
    // Ý nghĩa: ma trận đơn vị n x n theo mod.
    // Tham số: n = cỡ; mod = modulo.
    // Trả về: ma trận đơn vị.
    static Matrix identity(int n, long long mod) {
        Matrix I(n, mod);
        for (int i = 0; i < n; i++) I.a[i][i] = 1 % mod;
        return I;
    }
    // Ý nghĩa: nhân hai ma trận theo mod.
    // Tham số: o = ma trận bên phải.
    // Trả về: tích (this * o) theo mod.
    Matrix operator*(const Matrix& o) const {
        Matrix r(n, mod);
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++) {
                if (!a[i][k]) continue;
                for (int j = 0; j < n; j++)
                    r.a[i][j] = (r.a[i][j] + (__int128)a[i][k] * o.a[k][j]) % mod;
            }
        return r;
    }
    // Ý nghĩa: lũy thừa ma trận bằng bình phương liên tiếp.
    // Tham số: e = số mũ (>= 0).
    // Trả về: this^e theo mod.
    Matrix pow(long long e) const {
        Matrix r = identity(n, mod), b = *this;
        while (e > 0) { if (e & 1) r = r * b; b = b * b; e >>= 1; }
        return r;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ matrix-exponentiation.hpp -o demo && ./demo
int main() {
    const long long MOD = 1000000007;
    Matrix M(2, MOD);
    M.a = {{1, 1}, {1, 0}};   // ma trận Fibonacci
    Matrix R = M.pow(10);
    printf("Fibonacci(10) = %lld\n", R.a[0][1]);   // 55
    return 0;
}
#endif
