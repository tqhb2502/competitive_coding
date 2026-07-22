// Signal Processing - https://cses.fi/problemset/task/2113
//
// Truot mask qua signal; tai moi vi tri cong tong tich cac phan tu chong lap.
// Ket qua chinh la convolution cua signal a voi mask b DAO NGUOC (reversed):
//     result[p] = sum_{i+j=p} a[i] * b_rev[j]
// Tinh bang NTT. Gia tri toi da mot he so <= min(n,m)*100*100 = 2*10^9 < MOD,
// nen mot prime NTT (MOD = 15*2^27+1 = 2013265921, primitive root 31) la du,
// khong can CRT.
#include <bits/stdc++.h>
using namespace std;

const long long MOD = 2013265921LL; // 15 * 2^27 + 1
const long long G   = 31;           // primitive root cua MOD

long long power(long long a, long long b) {
    a %= MOD;
    long long r = 1;
    while (b > 0) {
        if (b & 1) r = (__int128)r * a % MOD;
        a = (__int128)a * a % MOD;
        b >>= 1;
    }
    return r;
}

void ntt(vector<long long>& a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        long long w = invert ? power(G, MOD - 1 - (MOD - 1) / len)
                             : power(G, (MOD - 1) / len);
        for (int i = 0; i < n; i += len) {
            long long wn = 1;
            int half = len >> 1;
            for (int k = 0; k < half; k++) {
                long long u = a[i + k];
                long long v = (__int128)a[i + k + half] * wn % MOD;
                a[i + k]        = u + v < MOD ? u + v : u + v - MOD;
                a[i + k + half] = u - v >= 0 ? u - v : u - v + MOD;
                wn = (__int128)wn * w % MOD;
            }
        }
    }
    if (invert) {
        long long n_inv = power(n, MOD - 2);
        for (long long& x : a) x = (__int128)x * n_inv % MOD;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<long long> a(n), b(m);
    for (auto& x : a) cin >> x;
    for (auto& x : b) cin >> x;

    reverse(b.begin(), b.end()); // -> convolution chuan

    int resSize = n + m - 1;
    int sz = 1;
    while (sz < resSize) sz <<= 1;
    a.resize(sz, 0);
    b.resize(sz, 0);

    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < sz; i++) a[i] = (__int128)a[i] * b[i] % MOD;
    ntt(a, true);

    string out;
    out.reserve(resSize * 7);
    for (int i = 0; i < resSize; i++) {
        if (i) out.push_back(' ');
        out += to_string(a[i]);
    }
    out.push_back('\n');
    cout << out;
    return 0;
}
