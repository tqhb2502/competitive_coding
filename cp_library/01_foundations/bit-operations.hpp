#pragma once
#include <bits/stdc++.h>
using namespace std;
// Bit Operations — đóng gói bit, Hamming/popcount, đếm bit tiền tố và kiểm submask/XOR-grid.
// Khi dùng: trạng thái vừa trong 64 bit; khoảng cách Hamming; tổng số bit 1 trên [0,n]; công thức theo bit/XOR.
// ĐPT: pack O(|s|), countSetBitsUpTo O(64), các phép còn lại O(1); bộ nhớ O(1).
// Dùng: packBinary64(s); hammingDistance64(a,b); countSetBitsUpTo(n); isSubmaskBits(x,mask);
// Bẫy: packBinary64 chỉ nhận tối đa 64 ký tự '0'/'1'; tổng bit tới n=2^64-1 cần unsigned __int128.
// CSES: 1146 1157 2136 2419 3228 3419
// Ý nghĩa: đóng gói xâu nhị phân (ký tự đầu là bit cao) vào uint64_t.
// Tham số: s = xâu gồm '0'/'1', dài không quá 64.
// Trả về: giá trị đóng gói; ném invalid_argument/length_error nếu dữ liệu sai.
uint64_t packBinary64(const string& s) {
    if (s.size() > 64) throw length_error("packBinary64: quá 64 bit");
    uint64_t value = 0;
    for (char c : s) {
        if (c != '0' && c != '1') throw invalid_argument("packBinary64: ký tự không phải bit");
        value = (value << 1) | uint64_t(c - '0');
    }
    return value;
}

// Ý nghĩa: tính khoảng cách Hamming giữa hai word 64 bit.
// Tham số: a, b = hai word cần so.
// Trả về: số vị trí bit khác nhau.
int hammingDistance64(uint64_t a, uint64_t b) {
    return __builtin_popcountll(a ^ b);
}

// Ý nghĩa: đếm tổng số bit 1 trong biểu diễn nhị phân của mọi số thuộc [0, n].
// Tham số: n = biên phải đóng.
// Trả về: tổng kiểu unsigned __int128 để không tràn cả khi n=2^64-1.
unsigned __int128 countSetBitsUpTo(uint64_t n) {
    unsigned __int128 total = 0;
    unsigned __int128 amount = (unsigned __int128)n + 1;
    for (int bit = 0; bit < 64; ++bit) {
        unsigned __int128 half = (unsigned __int128)1 << bit;
        unsigned __int128 block = half << 1;
        total += (amount / block) * half;
        unsigned __int128 rem = amount % block;
        if (rem > half) total += rem - half;
    }
    return total;
}

// Ý nghĩa: kiểm tra sub có phải mặt nạ con của mask hay không.
// Tham số: sub = mặt nạ ứng viên; mask = mặt nạ chứa.
// Trả về: true khi mọi bit bật của sub cũng bật trong mask.
bool isSubmaskBits(uint64_t sub, uint64_t mask) {
    return (sub & ~mask) == 0;
}

// Ý nghĩa: giá trị mex-grid chuẩn tại hàng row, cột col (đều 0-based).
// Tham số: row, col = chỉ số không âm.
// Trả về: row XOR col.
uint64_t xorMexGridValue(uint64_t row, uint64_t col) {
    return row ^ col;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ bit-operations.hpp -o demo && ./demo
int main() {
    uint64_t a = packBinary64("10110"), b = packBinary64("10001");
    printf("Hamming=%d; bit-1 trong [0,7]=%llu; mex(5,3)=%llu\n",
           hammingDistance64(a, b), (unsigned long long)countSetBitsUpTo(7),
           (unsigned long long)xorMexGridValue(5, 3));  // 3, 12, 6
    return 0;
}
#endif
