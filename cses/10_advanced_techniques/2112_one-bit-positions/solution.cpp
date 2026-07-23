#include <bits/stdc++.h>
using namespace std;

const long long MOD = 998244353;
const long long G = 3; // căn nguyên thủy của 998244353

// Lũy thừa nhanh theo modulo, dùng cho căn đơn vị và nghịch đảo trong NTT
long long power_mod(long long a, long long b, long long m) {
    long long r = 1 % m;
    a %= m;
    while (b) {
        if (b & 1) r = r * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return r;
}

// Biến đổi NTT tại chỗ (mod 998244353); invert = true để lấy biến đổi ngược
void ntt(vector<long long> &a, bool invert) {
    int n = (int)a.size();
    // Sắp xếp lại các phần tử theo thứ tự bit-reversal
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    // Gộp các đoạn tăng dần theo phép biến đổi butterfly
    for (int len = 2; len <= n; len <<= 1) {
        // w là căn bậc len của đơn vị (hoặc nghịch đảo của nó khi biến đổi ngược)
        long long w = invert ? power_mod(G, MOD - 1 - (MOD - 1) / len, MOD)
                             : power_mod(G, (MOD - 1) / len, MOD);
        for (int i = 0; i < n; i += len) {
            long long wn = 1;
            int half = len >> 1;
            for (int k = 0; k < half; k++) {
                long long u = a[i + k];
                long long v = a[i + k + half] * wn % MOD;
                a[i + k] = (u + v) % MOD;
                a[i + k + half] = (u - v + MOD) % MOD;
                wn = wn * w % MOD;
            }
        }
    }
    // Biến đổi ngược cần chia toàn bộ mảng cho n (nhân nghịch đảo của n)
    if (invert) {
        long long ninv = power_mod(n, MOD - 2, MOD);
        for (auto &x : a) x = x * ninv % MOD;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    if (!(cin >> s)) return 0;
    int n = (int)s.size();
    if (n <= 1) return 0; // theo ràng buộc n >= 2; nếu n <= 1 thì không in gì

    // Chọn kích thước sz là lũy thừa của 2 và >= 2n để tránh wrap-around
    int sz = 1;
    while (sz < 2 * n) sz <<= 1;

    // a là dãy bit s, b là bản đảo ngược của s; tích chập của chúng cho tự tương quan
    vector<long long> a(sz, 0), b(sz, 0);
    for (int i = 0; i < n; i++) {
        a[i] = (s[i] == '1') ? 1 : 0;
        b[i] = (s[n - 1 - i] == '1') ? 1 : 0; // bản đảo ngược của s
    }

    // Tính tích chập: NTT thuận hai dãy, nhân điểm-điểm, rồi NTT ngược
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < sz; i++) a[i] = a[i] * b[i] % MOD;
    ntt(a, true);

    // conv[m] = A[m - (n-1)]. Cần in A[k], k = 1..n-1  ->  m = n .. 2n-2.
    string out;
    out.reserve((size_t)(n - 1) * 7);
    for (int k = 1; k <= n - 1; k++) {
        int m = (n - 1) + k;
        long long val = a[m]; // đã là số nguyên đúng vì < MOD
        out += to_string(val);
        out += (k == n - 1) ? '\n' : ' ';
    }
    cout << out;
    return 0;
}
