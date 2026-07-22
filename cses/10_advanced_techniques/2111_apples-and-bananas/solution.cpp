// Apples and Bananas - https://cses.fi/problemset/task/2111
// Bai toan: dem so cach chon 1 tao va 1 chuoi co tong trong luong = w, voi moi w tu 2 den 2k.
// Day chinh la tich chap (convolution) giua mang dem trong luong tao va mang dem trong luong chuoi.
// Ket qua co the len toi n*m = 4e10 > 1 so nguyen to NTT (~1e9), nen dung 2 so nguyen to NTT + CRT
// de khoi phuc gia tri chinh xac. Ca hai modulo < 1.01e9 nen moi tich hai he so < 1.01e18, vua
// trong unsigned long long -> dung modmul 64-bit (nhanh hon __int128) va precompute twiddle.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;

const ll MOD1 = 998244353;   // 119 * 2^23 + 1
const ll MOD2 = 1004535809;  // 479 * 2^21 + 1
const ll G = 3;              // can nguyen thuy cho ca hai so nguyen to

ll power(ll a, ll b, ll mod) {
    a %= mod;
    if (a < 0) a += mod;
    ll r = 1;
    while (b) {
        if (b & 1) r = (ull)r * a % mod;
        a = (ull)a * a % mod;
        b >>= 1;
    }
    return r;
}

// NTT tai cho, cac he so < mod < 1.01e9 nen (ull)x*y an toan (< 1.01e18 < 2^64).
void ntt(vector<ll>& a, bool inv, ll mod) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    vector<ll> wp(n >> 1);
    for (int len = 2; len <= n; len <<= 1) {
        int half = len >> 1;
        ll ang = power(G, (mod - 1) / len, mod);
        if (inv) ang = power(ang, mod - 2, mod);
        // precompute twiddle factors cho tang nay -> bo 1 modmul/butterfly
        wp[0] = 1;
        for (int j = 1; j < half; j++) wp[j] = (ull)wp[j - 1] * ang % mod;
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < half; j++) {
                ll u = a[i + j];
                ll v = (ull)a[i + j + half] * (ull)wp[j] % mod;
                ll s = u + v; if (s >= mod) s -= mod;
                ll d = u - v; if (d < 0) d += mod;
                a[i + j] = s;
                a[i + j + half] = d;
            }
        }
    }
    if (inv) {
        ll ninv = power(n, mod - 2, mod);
        for (auto& x : a) x = (ull)x * ninv % mod;
    }
}

vector<ll> multiply(vector<ll> a, vector<ll> b, ll mod) {
    int rs = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < rs) n <<= 1;
    a.resize(n, 0);
    b.resize(n, 0);
    ntt(a, false, mod);
    ntt(b, false, mod);
    for (int i = 0; i < n; i++) a[i] = (ull)a[i] * b[i] % mod;
    ntt(a, true, mod);
    a.resize(rs);
    return a;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, n, m;
    cin >> k >> n >> m;

    vector<ll> A(k + 1, 0), B(k + 1, 0);
    for (int i = 0; i < n; i++) { int x; cin >> x; A[x]++; }
    for (int i = 0; i < m; i++) { int x; cin >> x; B[x]++; }

    vector<ll> c1 = multiply(A, B, MOD1);
    vector<ll> c2 = multiply(A, B, MOD2);

    // CRT: gia tri that = r1 + MOD1 * ((r2 - r1) * inv(MOD1) mod MOD2)
    ll inv1 = power(MOD1 % MOD2, MOD2 - 2, MOD2);

    int sz1 = (int)c1.size();
    int sz2 = (int)c2.size();

    string out;
    out.reserve((size_t)(2 * k) * 12);
    for (int w = 2; w <= 2 * k; w++) {
        ll r1 = (w < sz1) ? c1[w] : 0;
        ll r2 = (w < sz2) ? c2[w] : 0;
        ll t = ((r2 - r1) % MOD2 + MOD2) % MOD2;
        t = (ull)t * inv1 % MOD2;
        __int128 val = (__int128)r1 + (__int128)MOD1 * t; // < 4e10, vua trong long long
        out += to_string((ll)val);
        out += (w == 2 * k) ? '\n' : ' ';
    }
    cout << out;
    return 0;
}
