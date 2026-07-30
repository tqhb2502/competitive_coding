#pragma once
#include <bits/stdc++.h>
using namespace std;
// Gray Code — sinh dãy 2^n mã Gray (hai mã kề nhau khác đúng 1 bit) và chuyển đổi số ↔ mã Gray.
// Khi dùng: liệt kê mọi xâu nhị phân độ dài n sao cho hai xâu liên tiếp khác 1 bit; đánh số/giải mã vòng Gray.
// ĐPT: grayCode O(2^n · n) thời gian, bộ nhớ O(2^n · n) cho kết quả; grayEncode O(1); grayDecode = grayRank O(log W) = 6 phép xor với W = 64 bit.
// Dùng: for (const string& s : grayCode(3)) cout << s << '\n';  // 000 001 011 010 110 111 101 100
//        unsigned long long g = grayEncode(5); grayDecode(g) == 5; grayRank(g) == 5;
// Bẫy: grayCode giữ CẢ 2^n xâu trong RAM — n <= 16 (CSES) là 64 Ki xâu; n lớn hơn nên in trực tiếp từ grayEncode(i).
//       grayDecode KHÔNG phải g ^ (g >> 1) mà là xor mọi hậu tố dịch phải (g>>1 ^ g>>2 ^ ...).
// CSES: 2205
// Ý nghĩa: mã Gray phản xạ của số thứ tự x (mã thứ x trong vòng Gray, 0-based).
// Tham số: x = thứ tự 0-based của mã cần lấy (mọi giá trị 64 bit không dấu đều hợp lệ).
// Trả về: x ^ (x >> 1) — mã Gray tại vị trí x; grayEncode(x) và grayEncode(x+1) khác đúng 1 bit.
inline unsigned long long grayEncode(unsigned long long x) { return x ^ (x >> 1); }

// Ý nghĩa: giải mã Gray về số thứ tự, bằng cách xor dồn mọi phép dịch phải (tương đương xor tiền tố các bit cao).
// Tham số: g = mã Gray 64 bit cần giải.
// Trả về: x duy nhất thoả grayEncode(x) == g (nghịch đảo hoàn toàn của grayEncode).
inline unsigned long long grayDecode(unsigned long long g) {
    for (int sh = 1; sh < 64; sh <<= 1) g ^= g >> sh;
    return g;
}

// Ý nghĩa: thứ hạng của mã g trong vòng Gray, tức số bước đi từ mã 0 tới g (đúng bằng grayDecode(g)).
// Tham số: g = mã Gray 64 bit cần tra thứ tự.
// Trả về: chỉ số 0-based i sao cho g là mã Gray thứ i (grayCode(n)[grayRank(g)] biểu diễn g khi g < 2^n).
inline unsigned long long grayRank(unsigned long long g) { return grayDecode(g); }

// Ý nghĩa: dựng toàn bộ dãy 2^n mã Gray dưới dạng xâu n ký tự '0'/'1' (bit cao bên trái) — đúng định dạng CSES 2205.
// Tham số: n = số bit, 0 <= n <= 62 (n = 0 cho đúng một xâu rỗng).
// Trả về: vector 2^n xâu độ dài n, là hoán vị của mọi xâu nhị phân độ dài n; hai xâu liền kề (kể cả cặp đầu–cuối, vòng tròn) khác đúng 1 bit.
inline vector<string> grayCode(int n) {
    unsigned long long total = 1ULL << n;
    vector<string> res;
    res.reserve((size_t)total);
    string buf(n, '0');
    for (unsigned long long i = 0; i < total; i++) {
        unsigned long long g = grayEncode(i);
        for (int b = 0; b < n; b++) buf[n - 1 - b] = ((g >> b) & 1ULL) ? '1' : '0';
        res.push_back(buf);
    }
    return res;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ gray-code.hpp -o demo && ./demo
int main() {
    for (const string& s : grayCode(3)) printf("%s ", s.c_str());
    printf("\n");  // 000 001 011 010 110 111 101 100
    printf("%llu %llu %llu\n", grayEncode(5), grayDecode(grayEncode(5)), grayRank(7));
    // 7 5 5   (grayEncode(5) = 5 ^ 2 = 7; giai ma tro lai 5; ma 7 = "111" dung o vi tri 5)
    printf("%zu %s\n", grayCode(16).size(), grayCode(16).back().c_str());
    // 65536 1000000000000000   (ma cuoi cua n = 16 chi khac ma dau "000...0" dung 1 bit)
    return 0;
}
#endif
