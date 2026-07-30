#pragma once
#include <bits/stdc++.h>
using namespace std;
// Ternary / Convex Search — tìm cực TIỂU hàm trên miền SỐ NGUYÊN, O(log(hi-lo)): ternaryMin (cần NGẶT) và convexMin.
// Khi dùng: cần điểm/giá trị nhỏ nhất của hàm "giảm rồi tăng" mà không có công thức đóng (tìm MAX thì dùng với -f).
// ĐPT: O(log(hi-lo)) lần gọi f; bộ nhớ O(1). convexMin chặt nhị phân trên f(m) <= f(m+1) nên CHỊU ĐƯỢC đoạn bằng nhau.
// Dùng: ternaryMin(lo, hi, f); // f giảm NGẶT rồi tăng NGẶT | convexMin(lo, hi, f); // f LỒI, cho phép plateau
// Bẫy: ternaryMin SAI IM LẶNG nếu f có đoạn bằng nhau -> dùng convexMin (cần LỒI THẬT); hàm chỉ "gần lồi" như ceil(A/x)+B*x thì CẢ HAI có thể sai, phải quét thêm lân cận (±10).
// CSES: 1074
// Ý nghĩa: tìm điểm nguyên trong [lo, hi] làm f nhỏ nhất, giả thiết f GIẢM NGẶT rồi TĂNG NGẶT (không có đoạn bằng nhau).
// Tham số: lo, hi = biên miền tìm kiếm (đóng); f = hàm đơn điệu một đỉnh NGẶT nhận long long.
// Trả về: giá trị x đạt cực tiểu của f trên [lo, hi] (SAI nếu f có plateau ngoài đáy — khi đó dùng convexMin).
template <class F>
long long ternaryMin(long long lo, long long hi, F f) {
    while (hi - lo > 2) {
        long long m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) hi = m2 - 1;
        else lo = m1 + 1;
    }
    long long best = lo;
    for (long long x = lo + 1; x <= hi; x++) if (f(x) < f(best)) best = x;
    return best;
}
// Ý nghĩa: tìm điểm cực tiểu của hàm LỒI bằng chặt nhị phân trên điều kiện f(m) <= f(m+1); đúng cả khi f có plateau.
// Tham số: lo, hi = biên miền tìm kiếm (đóng); f = hàm LỒI THẬT — hiệu bậc một f(x+1)-f(x) KHÔNG GIẢM.
// Trả về: x NHỎ NHẤT đạt cực tiểu của f trên [lo, hi].
// Lưu ý: nếu f chỉ "gần lồi" (vd ceil(A/x)+B*x — đã đo là KHÔNG lồi) thì hãy quét thêm ±10 quanh kết quả:
//        đo trên A <= 2000, B <= 8 -> quét ±5 vẫn sai 144/16000 ca, quét ±10 thì đúng 16000/16000.
template <class F>
long long convexMin(long long lo, long long hi, F f) {
    while (lo < hi) {
        long long m = lo + (hi - lo) / 2;   // chia đôi tránh tràn
        if (f(m) <= f(m + 1)) hi = m;       // đã hết đoạn giảm -> cực tiểu nằm trong [lo, m]
        else lo = m + 1;
    }
    return lo;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ ternary-search.hpp -o demo && ./demo
int main() {
    auto f = [](long long x) { return (x - 3) * (x - 3) + 7; };   // parabol: giảm ngặt rồi tăng ngặt
    long long x = ternaryMin(-10, 10, f);
    // Bảng có PLATEAU ngoài đáy: ternaryMin sai, convexMin đúng.
    static const long long tab[6] = {2, 1, 0, 1, 1, 1};
    auto p = [&](long long i) { return tab[i]; };
    printf("ternaryMin parabol: x=%lld f=%lld | plateau: ternaryMin f=%lld (SAI) vs convexMin f=%lld (ĐÚNG)\n",
           x, f(x), p(ternaryMin(0, 5, p)), p(convexMin(0, 5, p)));   // x=3 f=7 | 1 vs 0
    return 0;
}
#endif
