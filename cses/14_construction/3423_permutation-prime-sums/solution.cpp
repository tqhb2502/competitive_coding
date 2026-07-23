#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    // Sàng nguyên tố tới 2n vì số nguyên tố cần dùng tối đa là 2n.
    int M = 2 * n;
    if (M < 2) M = 2;
    vector<char> comp(M + 1, 0);
    comp[0] = 1;
    comp[1] = 1;
    for (int i = 2; (long long)i * i <= M; i++) {
        if (!comp[i])
            for (int j = i * i; j <= M; j += i) comp[j] = 1;
    }

    // prevPrime[x] = số nguyên tố lớn nhất không vượt quá x.
    vector<int> prevPrime(M + 1, 0);
    int last = 0;
    for (int x = 0; x <= M; x++) {
        if (!comp[x]) last = x;
        prevPrime[x] = last;
    }

    // Xây dựng hoán vị sigma (mảng b) bằng đệ quy theo khối.
    // Cố định a_i = i, cần i + b_i là số nguyên tố với mọi i.
    vector<int> b(n + 1);
    int m = n;
    while (m >= 1) {
        int p = prevPrime[2 * m];   // số nguyên tố trong khoảng (m, 2m] theo định đề Bertrand
        int lo = p - m;             // luôn có 1 <= lo <= m
        // Ghép đối xứng khối con [lo, m]: mỗi cặp có tổng i + b_i = p là số nguyên tố.
        for (int i = lo; i <= m; i++) b[i] = p - i;
        m = lo - 1;                 // đệ quy tiếp với khối còn lại [1, lo-1]
    }

    // Xuất kết quả: dòng 1 là a = 1..n, dòng 2 là b = sigma. In nhanh bằng bộ đệm.
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
