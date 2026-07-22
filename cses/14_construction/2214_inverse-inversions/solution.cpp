// Inverse Inversions - CSES 2214
// https://cses.fi/problemset/task/2214
//
// Xay dung hoan vi cua 1..n co dung k inversion.
// Tham lam tu trai sang phai, gia tri chua dung luon la doan lien tuc [lo, hi].
// Nghiem luon ton tai vi 0 <= k <= n(n-1)/2, khong co truong hop IMPOSSIBLE.

#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n, k;
    // Doc n va k
    if (scanf("%lld %lld", &n, &k) != 2) return 0;

    vector<int> res;
    res.reserve((size_t)n);

    long long lo = 1, hi = n;
    while (lo <= hi) {
        if (k <= 0) {
            // Xa phan con lai theo thu tu tang dan (them 0 inversion)
            for (long long v = lo; v <= hi; ++v) res.push_back((int)v);
            break;
        }
        long long m = hi - lo + 1;      // so gia tri chua dung
        if (k >= m - 1) {
            // Dat gia tri lon nhat -> tao them m-1 inversion
            res.push_back((int)hi);
            hi--;
            k -= (m - 1);
        } else {
            // 0 < k < m-1: dat lo+k de dong gop dung k inversion,
            // phan con lai xep tang dan (tru gia tri da dung)
            long long chosen = lo + k;
            res.push_back((int)chosen);
            for (long long v = lo; v <= hi; ++v)
                if (v != chosen) res.push_back((int)v);
            k = 0;
            break;
        }
    }

    // Xuat ket qua voi buffer nhanh
    string out;
    out.reserve(res.size() * 7 + 1);
    char buf[16];
    for (size_t i = 0; i < res.size(); ++i) {
        if (i) out.push_back(' ');
        int x = res[i];
        int len = 0;
        if (x == 0) buf[len++] = '0';
        while (x > 0) { buf[len++] = char('0' + x % 10); x /= 10; }
        while (len > 0) out.push_back(buf[--len]);
    }
    out.push_back('\n');
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
