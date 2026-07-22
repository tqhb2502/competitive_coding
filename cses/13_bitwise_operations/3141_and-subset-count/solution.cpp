// And Subset Count (CSES 3141)
// https://cses.fi/problemset/task/3141
//
// Với mỗi k = 0..n, đếm số tập con không rỗng có bitwise AND = k (mod 1e9+7).
// Kỹ thuật: superset-sum SOS DP + Möbius inversion trên lưới tập con bit.
//   g[m]        = số phần tử là superset của mask m  (SOS superset sum)
//   atleast[m]  = 2^{g[m]} - 1  = số tập con không rỗng có AND là superset của m
//   exact[k]    = superset Möbius inversion của atleast

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    const int B = 18;            // 2^18 = 262144 > 2*10^5 >= mọi a_i và mọi k
    const int SIZE = 1 << B;

    // cnt / g: số phần tử bằng mỗi giá trị (sau đó biến thành superset-sum)
    vector<int> g(SIZE, 0);
    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;                // 0 <= a <= n < SIZE (a_i nằm trong đoạn hợp lệ)
        g[a]++;
    }

    // Superset sum SOS: g[m] = số phần tử a_i thỏa (a_i & m) == m.
    for (int b = 0; b < B; b++) {
        int bit = 1 << b;
        for (int m = 0; m < SIZE; m++) {
            if (!(m & bit)) {
                g[m] += g[m | bit];
            }
        }
    }

    // Bảng lũy thừa của 2 tới n (vì g[m] <= n).
    vector<long long> pw(n + 1);
    pw[0] = 1;
    for (int i = 1; i <= n; i++) pw[i] = pw[i - 1] * 2 % MOD;

    // atleast[m] = 2^{g[m]} - 1
    vector<long long> f(SIZE);
    for (int m = 0; m < SIZE; m++) {
        f[m] = (pw[g[m]] - 1 + MOD) % MOD;
    }

    // Superset Möbius inversion: biến atleast -> exact.
    for (int b = 0; b < B; b++) {
        int bit = 1 << b;
        for (int m = 0; m < SIZE; m++) {
            if (!(m & bit)) {
                f[m] = (f[m] - f[m | bit] + MOD) % MOD;
            }
        }
    }

    // In exact[0..n].
    // (in ra n+1 số, phân tách bởi dấu cách, kết thúc bằng xuống dòng)
    string out;
    out.reserve((size_t)(n + 1) * 12);
    for (int k = 0; k <= n; k++) {
        out += to_string(f[k]);
        out += (k == n ? '\n' : ' ');
    }
    cout << out;

    return 0;
}
