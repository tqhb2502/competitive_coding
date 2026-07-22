// All Subarray Xors (CSES 3233)
// https://cses.fi/problemset/task/3233
//
// XOR của đoạn con (l..r) = prefix[r] ^ prefix[l-1], nên tập tất cả các giá trị
// XOR đoạn con = { prefix[i] ^ prefix[j] : 0 <= j < i <= n }, tức tập XOR mọi cặp
// giá trị prefix. Vì x_i <= 10^6 < 2^20 nên mọi giá trị nằm trong [0, 2^20).
// Dùng Walsh-Hadamard Transform để tính XOR self-convolution của vector chỉ thị
// các prefix phân biệt: g[v] = số cặp (a,b) với a^b = v.
//  - v != 0 đạt được <=> g[v] > 0
//  - v = 0  đạt được <=> có prefix bị lặp (distinct < n+1)

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    const int LOG = 20;
    const int M = 1 << LOG;               // 1048576 > 10^6

    vector<long long> f(M, 0);            // vừa làm marker, vừa làm mảng WHT

    long long cur = 0;
    int distinct = 0;
    // prefix[0] = 0
    f[0] = 1;
    distinct = 1;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        cur ^= x;
        if (f[cur] == 0) {
            f[cur] = 1;
            distinct++;
        }
    }
    bool hasDup = (distinct < n + 1);     // có prefix lặp => XOR = 0 đạt được

    // Walsh-Hadamard Transform (thuận)
    for (int len = 1; len < M; len <<= 1) {
        for (int i = 0; i < M; i += (len << 1)) {
            for (int j = i; j < i + len; j++) {
                long long a = f[j], b = f[j + len];
                f[j] = a + b;
                f[j + len] = a - b;
            }
        }
    }
    // pointwise square
    for (int i = 0; i < M; i++) f[i] *= f[i];
    // WHT nghịch (áp dụng lại, sau đó chia M). Ta không cần chia vì chỉ kiểm tra > 0.
    for (int len = 1; len < M; len <<= 1) {
        for (int i = 0; i < M; i += (len << 1)) {
            for (int j = i; j < i + len; j++) {
                long long a = f[j], b = f[j + len];
                f[j] = a + b;
                f[j + len] = a - b;
            }
        }
    }
    // f[v] = M * (số cặp có thứ tự a^b = v) >= 0

    // Thu thập kết quả theo thứ tự tăng dần
    vector<int> res;
    if (hasDup) res.push_back(0);
    for (int v = 1; v < M; v++) {
        if (f[v] > 0) res.push_back(v);
    }

    // Xuất
    string out;
    out.reserve(res.size() * 7 + 16);
    out += to_string((long long)res.size());
    out += '\n';
    for (size_t i = 0; i < res.size(); i++) {
        if (i) out += ' ';
        out += to_string(res[i]);
    }
    out += '\n';
    cout << out;
    return 0;
}
