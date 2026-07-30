#pragma once
#include <bits/stdc++.h>
using namespace std;
// Sqrt Decomposition — chia mảng thành các khối ~sqrt(n) hỗ trợ CỘNG ĐOẠN + TỔNG ĐOẠN, O(sqrt(n)) mỗi thao tác.
// Khi dùng: range-update + range-query khi muốn code ngắn hơn lazy segment tree; nền cho nhiều biến thể khối.
// ĐPT: dựng O(n), mỗi thao tác O(sqrt(n)); bộ nhớ O(n). Khối lưu tổng + lazy cộng; chỉ số 0-based, đoạn [l, r] ĐÓNG.
// Dùng: SqrtDecomp sd(a); sd.rangeAdd(l, r, v); sd.rangeSum(l, r); // [l, r] bao gồm cả hai đầu
// Bẫy: đoạn ĐÓNG [l, r]; dùng long long cho tổng; khối biên xử lý từng phần, khối trọn dùng lazy.
// CSES: 1735
struct SqrtDecomp {
    int n, blk;
    vector<long long> a, blockSum, lazy;
    // Ý nghĩa: dựng cấu trúc từ mảng ban đầu, chia khối ~sqrt(n) và tính tổng từng khối.
    // Tham số: init = mảng giá trị ban đầu.
    explicit SqrtDecomp(const vector<long long>& init) : n((int)init.size()), a(init) {
        blk = max(1, (int)sqrt((double)max(1, n)));
        blockSum.assign((n + blk - 1) / blk + 1, 0);
        lazy.assign((n + blk - 1) / blk + 1, 0);
        for (int i = 0; i < n; i++) blockSum[i / blk] += a[i];
    }
    // Ý nghĩa: cộng v vào mọi phần tử trong đoạn đóng [l, r].
    // Tham số: l, r = biên đoạn (0-based, đóng); v = lượng cộng.
    void rangeAdd(int l, int r, long long v) {
        for (int i = l; i <= r;) {
            if (i % blk == 0 && i + blk - 1 <= r) { lazy[i / blk] += v; blockSum[i / blk] += v * blk; i += blk; }
            else { a[i] += v; blockSum[i / blk] += v; i++; }
        }
    }
    // Ý nghĩa: tổng các phần tử trong đoạn đóng [l, r].
    // Tham số: l, r = biên đoạn (0-based, đóng).
    // Trả về: tổng a[l..r] (đã tính lazy).
    long long rangeSum(int l, int r) {
        long long s = 0;
        for (int i = l; i <= r;) {
            if (i % blk == 0 && i + blk - 1 <= r) { s += blockSum[i / blk]; i += blk; }
            else { s += a[i] + lazy[i / blk]; i++; }
        }
        return s;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ sqrt-decomposition.hpp -o demo && ./demo
int main() {
    SqrtDecomp sd(vector<long long>{1, 2, 3, 4, 5});
    sd.rangeAdd(1, 3, 10);   // -> 1 12 13 14 5
    printf("sum[0,4]=%lld ; sum[1,3]=%lld\n", sd.rangeSum(0, 4), sd.rangeSum(1, 3));   // 45 ; 39
    return 0;
}
#endif
