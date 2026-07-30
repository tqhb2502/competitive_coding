#pragma once
#include <bits/stdc++.h>
using namespace std;
// String Hashing — băm đa thức HAI mod (giảm va chạm) cho hash mọi đoạn con trong O(1) sau tiền xử lý O(n).
// Khi dùng: so sánh nhanh hai đoạn con bằng nhau, đếm đoạn phân biệt, tìm mẫu, kiểm palindrome.
// ĐPT: dựng O(n), get O(1); bộ nhớ O(n). Hai đoạn bằng nhau <=> get() bằng nhau (xác suất va chạm cực nhỏ).
// Dùng: StringHash h(s); h.get(l, r); // hash của đoạn nửa mở s[l, r); so hai đoạn: h.get(l1,r1)==h.get(l2,r2)
// Bẫy: đoạn nửa mở [l, r); dùng HAI mod gộp 64-bit để tránh va chạm; base > mọi mã ký tự; không dùng cho input đối kháng cực đoan.
// CSES: 1753 2420
struct StringHash {
    static constexpr long long M1 = 1000000007, M2 = 998244353;
    long long B1 = 131, B2 = 137;
    vector<long long> h1, h2, p1, p2;
    // Ý nghĩa: tiền xử lý hash tiền tố và lũy thừa base cho hai mod từ xâu s.
    // Tham số: s = xâu cần băm.
    explicit StringHash(const string& s) {
        int n = (int)s.size();
        h1.assign(n + 1, 0); h2.assign(n + 1, 0);
        p1.assign(n + 1, 1); p2.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * B1 + s[i]) % M1;
            h2[i + 1] = (h2[i] * B2 + s[i]) % M2;
            p1[i + 1] = p1[i] * B1 % M1;
            p2[i + 1] = p2[i] * B2 % M2;
        }
    }
    // Ý nghĩa: lấy hash gộp 64-bit của đoạn nửa mở s[l, r).
    // Tham số: l = biên trái (bao gồm); r = biên phải (loại trừ).
    // Trả về: hash gộp (hai đoạn cho cùng giá trị <=> gần như chắc chắn bằng nhau).
    long long get(int l, int r) const {
        long long a = ((h1[r] - h1[l] * p1[r - l]) % M1 + M1) % M1;
        long long b = ((h2[r] - h2[l] * p2[r - l]) % M2 + M2) % M2;
        return a * M2 + b;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ string-hashing.hpp -o demo && ./demo
int main() {
    StringHash h("abcabc");
    printf("hash('abc'==s[0,3]) == hash(s[3,6]) ? %d ; == hash(s[1,4]) ? %d\n",
           (int)(h.get(0, 3) == h.get(3, 6)), (int)(h.get(0, 3) == h.get(1, 4)));   // 1 ; 0
    return 0;
}
#endif
