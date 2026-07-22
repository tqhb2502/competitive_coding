// Permutation Prime Sums — CSES 3423
// https://cses.fi/problemset/task/3423
//
// Cần hai hoán vị a, b của {1..n} sao cho a_i + b_i là prime với mọi i.
// Cố định a = (1,2,...,n) rồi tìm hoán vị sigma = b với i + sigma_i prime.
// Dùng định đề Bertrand: với m >= 1 luôn có prime p thoả m < p <= 2m.
// Với khối [1, m]: lấy p = prime lớn nhất <= 2m (Bertrand => p > m),
// ghép đối xứng khối con [p-m, m] bằng sigma_i = p - i (mỗi tổng = p prime),
// rồi đệ quy khối [1, p-m-1]. Bài toán luôn có lời giải (không IMPOSSIBLE).

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    // Sàng nguyên tố tới 2n (cần prime tối đa 2n).
    int M = 2 * n;
    if (M < 2) M = 2;
    vector<char> comp(M + 1, 0);
    comp[0] = 1;
    comp[1] = 1;
    for (int i = 2; (long long)i * i <= M; i++) {
        if (!comp[i])
            for (int j = i * i; j <= M; j += i) comp[j] = 1;
    }

    // prevPrime[x] = prime lớn nhất <= x.
    vector<int> prevPrime(M + 1, 0);
    int last = 0;
    for (int x = 0; x <= M; x++) {
        if (!comp[x]) last = x;
        prevPrime[x] = last;
    }

    // Xây dựng sigma (mảng b) theo đệ quy khối.
    vector<int> b(n + 1);
    int m = n;
    while (m >= 1) {
        int p = prevPrime[2 * m];   // prime trong (m, 2m] theo Bertrand
        int lo = p - m;             // 1 <= lo <= m
        for (int i = lo; i <= m; i++) b[i] = p - i;
        m = lo - 1;                 // đệ quy khối [1, lo-1]
    }

    // Xuất kết quả: a = 1..n, b = sigma. In nhanh bằng buffer.
    string out;
    out.reserve((size_t)n * 14);
    char buf[16];
    for (int i = 1; i <= n; i++) {
        int len = sprintf(buf, "%d", i);
        out.append(buf, len);
        out.push_back(i < n ? ' ' : '\n');
    }
    for (int i = 1; i <= n; i++) {
        int len = sprintf(buf, "%d", b[i]);
        out.append(buf, len);
        out.push_back(i < n ? ' ' : '\n');
    }
    fputs(out.c_str(), stdout);
    return 0;
}
