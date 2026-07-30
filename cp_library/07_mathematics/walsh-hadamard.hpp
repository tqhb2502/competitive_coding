#pragma once
#include <bits/stdc++.h>
using namespace std;
// Walsh-Hadamard Transform (WHT) — tích chập XOR: c[k] = sum_{i ^ j == k} a[i] * b[j], O(n log n).
// Khi dùng: đếm cặp/bộ theo XOR trên miền giá trị nhỏ (< 2^20), luỹ thừa tích chập XOR, tập giá trị XOR đạt được.
// ĐPT: walshHadamard O(n log n); xorConvolution O(n log n) (gồm cả pad lên luỹ thừa 2); bộ nhớ O(n).
// Dùng: auto c = xorConvolution(a, b);          // c[k] = sum_{i^j==k} a[i]*b[j], độ dài = luỹ thừa 2 >= max(|a|,|b|)
// Dùng: walshHadamard(a, false); ... walshHadamard(a, true);   // biến đổi tại chỗ, |a| PHẢI là luỹ thừa của 2
// Bẫy: xorConvolution pad hai mảng lên CÙNG luỹ thừa 2 nên độ dài kết quả là luỹ thừa 2 (không phải |a|+|b|-1);
//      mảng rỗng -> trả về rỗng; tràn số: trung gian tới n * (sum|a|) * (sum|b|), cần long long (hoặc mod trước).
//      Đổi sang OR: thuận a[i | bit] += a[i], nghịch a[i | bit] -= a[i] (zeta/Möbius trên tập con).
//      Đổi sang AND: thuận a[i] += a[i | bit], nghịch a[i] -= a[i | bit] (zeta/Möbius trên tập chứa).
// CSES: 3233
// Ý nghĩa: biến đổi Walsh-Hadamard tại chỗ (thuận hoặc nghịch) — biến tích chập XOR thành nhân từng vị trí.
// Tham số: a = mảng độ dài LUỸ THỪA CỦA 2 (hoặc rỗng); inverse = true để biến đổi ngược (chia n ở cuối).
void walshHadamard(vector<long long>& a, bool inverse) {
    int n = (int)a.size();
    if (n == 0) return;
    for (int len = 1; len < n; len <<= 1)
        for (int i = 0; i < n; i += len << 1)
            for (int j = i; j < i + len; j++) {
                long long u = a[j], v = a[j + len];
                a[j] = u + v;
                a[j + len] = u - v;
            }
    // Chỉ chia MỘT lần ở cuối (chia dần từng tầng có thể không chia hết); kết quả thật luôn chia hết cho n.
    if (inverse)
        for (long long& x : a) x /= n;
}
// Ý nghĩa: tích chập XOR của hai mảng bằng WHT (tự pad độ dài lên luỹ thừa của 2).
// Tham số: a, b = mảng hệ số, chỉ số CHÍNH LÀ giá trị XOR (0-based); độ dài tuỳ ý, phần thiếu coi như 0.
// Trả về: mảng c độ dài n = luỹ thừa 2 nhỏ nhất >= max(|a|, |b|), c[k] = sum_{i ^ j == k} a[i] * b[j].
vector<long long> xorConvolution(vector<long long> a, vector<long long> b) {
    if (a.empty() || b.empty()) return {};
    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;
    a.resize(n, 0);
    b.resize(n, 0);
    walshHadamard(a, false);
    walshHadamard(b, false);
    for (int i = 0; i < n; i++) a[i] *= b[i];
    walshHadamard(a, true);
    return a;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ walsh-hadamard.hpp -o demo && ./demo
int main() {
    auto c = xorConvolution({1, 2, 3, 4}, {5, 6, 7, 8});
    printf("tích chập XOR: ");
    for (long long x : c) printf("%lld ", x);   // 70 68 62 60
    printf("\n");
    auto d = xorConvolution({1, 2, 3}, {1, 1});   // pad lên độ dài 4
    printf("pad luỹ thừa 2: ");
    for (long long x : d) printf("%lld ", x);   // 3 3 3 3
    printf("\n");
    // CSES 3233: dãy 5 1 5 9 -> prefix XOR {0, 5, 4, 1, 8}; tập XOR mọi đoạn con = XOR mọi cặp prefix.
    vector<long long> f(16, 0);
    for (int p : {0, 5, 4, 1, 8}) f[p] = 1;
    auto g = xorConvolution(f, f);
    int distinctXor = 0;
    for (int v = 1; v < 16; v++) if (g[v] > 0) distinctXor++;
    printf("số giá trị XOR đoạn con khác 0: %d\n", distinctXor);   // 7
    return 0;
}
#endif
