#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;

    const int B = 30;              // x_i <= 1e9 < 2^30
    vector<int> basis(B, 0);

    // Xây dựng XOR linear basis: basis[b] là vector có bit pivot cao nhất là b.
    for (int idx = 0; idx < n; idx++) {
        int x;
        cin >> x;
        for (int b = B - 1; b >= 0; b--) {
            if (!((x >> b) & 1)) continue;
            if (!basis[b]) { basis[b] = x; break; }
            x ^= basis[b];
        }
    }

    // Đưa về reduced row echelon form: bit pivot i chỉ tồn tại ở riêng basis[i].
    for (int i = 0; i < B; i++) {
        if (!basis[i]) continue;
        for (int j = 0; j < B; j++) {
            if (j == i) continue;
            if (basis[j] & (1 << i)) basis[j] ^= basis[i];
        }
    }

    // Thu thập các vector basis theo pivot tăng dần -> b_0 < b_1 < ...
    vector<int> sb;
    for (int i = 0; i < B; i++) if (basis[i]) sb.push_back(basis[i]);
    int r = (int)sb.size();

    // Bội số m = 2^(n-r), chặn lại vì n-r có thể rất lớn (k <= 2*10^5).
    long long m;
    int e = n - r;
    if (e >= 40) m = (long long)4e18;   // chắc chắn >= k
    else m = 1LL << e;

    // Liệt kê các giá trị phân biệt theo thứ tự tăng dần qua value(i).
    // value(i) = XOR các vector sb[b] với b là các bit đã bật của i; hàm này
    // tăng nghiêm ngặt theo i, nên duyệt i = 0, 1, 2, ... cho thứ tự tăng dần.
    string out;
    out.reserve((size_t)k * 11 + 16);

    long long remaining = k;
    long long i = 0;
    while (remaining > 0) {
        // Tính value(i): XOR các vector basis ứng với từng bit đã bật của i.
        long long ii = i;
        int val = 0;
        while (ii) {
            int b = __builtin_ctzll(ii);
            val ^= sb[b];
            ii &= ii - 1;
        }
        // Mỗi giá trị phân biệt lặp m lần trong multiset -> in min(m, còn_lại) bản sao.
        long long copies = min(m, remaining);
        string s = to_string(val);
        for (long long c = 0; c < copies; c++) {
            out += s;
            out += ' ';
        }
        remaining -= copies;
        i++;
    }

    if (!out.empty() && out.back() == ' ') out.back() = '\n';
    fputs(out.c_str(), stdout);
    return 0;
}
