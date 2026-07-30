#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    const int LOG = 20;
    const int M = 1 << LOG;               // 1048576 > 10^6, chứa mọi giá trị prefix

    vector<long long> f(M, 0);            // vừa là vector chỉ thị, vừa là mảng WHT

    // Đánh dấu các giá trị prefix PHÂN BIỆT: f[v] = 1 nếu v xuất hiện trong dãy prefix
    long long cur = 0;
    int distinct = 0;
    f[0] = 1;                             // prefix[0] = 0
    distinct = 1;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        cur ^= x;                         // cur = prefix[i+1]
        if (f[cur] == 0) {
            f[cur] = 1;
            distinct++;
        }
    }
    bool hasDup = (distinct < n + 1);     // có prefix bị lặp => tồn tại đoạn con XOR = 0

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
    // Bình phương từng điểm: tương ứng XOR self-convolution trong miền biến đổi
    for (int i = 0; i < M; i++) f[i] *= f[i];
    // WHT nghịch (áp dụng lại). Bỏ qua bước chia M vì chỉ cần kiểm tra dấu > 0.
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
    if (hasDup) res.push_back(0);         // giá trị 0 chỉ đạt khi có prefix lặp
    for (int v = 1; v < M; v++) {
        if (f[v] > 0) res.push_back(v);   // v != 0 đạt được khi có cặp prefix cho ra v
    }

    // Xuất số lượng rồi tới các giá trị
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
