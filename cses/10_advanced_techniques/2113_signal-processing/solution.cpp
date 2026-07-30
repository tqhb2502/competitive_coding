#include <bits/stdc++.h>
using namespace std;

// Prime NTT: MOD lớn hơn giá trị tối đa của hệ số đầu ra (khoảng 2*10^9) nên
// một modulo là đủ, không cần CRT.
const long long MOD = 2013265921LL; // 15 * 2^27 + 1
const long long G   = 31;           // primitive root của MOD

// Lũy thừa nhanh mod MOD, dùng __int128 để tránh tràn ở phép nhân trung gian.
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

// NTT thuận/nghịch (invert = true là biến đổi ngược).
void ntt(vector<long long>& a, bool invert) {
    int n = (int)a.size();
    // Hoán vị bit-reversal để đưa về thứ tự thích hợp cho biến đổi tại chỗ.
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    // Trộn theo từng tầng độ dài len, dùng căn đơn vị bậc len của MOD.
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
    // Với biến đổi ngược, chia đều cho n (nhân nghịch đảo của n mod MOD).
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

    // Đảo mask để bài toán trở thành convolution chuẩn (nhân hai đa thức).
    reverse(b.begin(), b.end());

    // Chọn kích thước biến đổi là lũy thừa 2 nhỏ nhất >= n+m-1, rồi pad 0.
    int resSize = n + m - 1;
    int sz = 1;
    while (sz < resSize) sz <<= 1;
    a.resize(sz, 0);
    b.resize(sz, 0);

    // Nhân đa thức: NTT thuận -> nhân từng điểm -> NTT nghịch.
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < sz; i++) a[i] = (__int128)a[i] * b[i] % MOD;
    ntt(a, true);

    // In n+m-1 hệ số đầu tiên, cách nhau bằng dấu cách.
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
