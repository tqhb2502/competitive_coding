#pragma once
#include <bits/stdc++.h>
using namespace std;
// Divisor Blocks — chia khối theo thương floor(n/i): mọi tổng dạng sum_{i=1..n} f(i)*floor(n/i) trong O(sqrt n).
// Khi dùng: tổng số ước / tổng các ước của mọi số trong [1, n], đếm bội (chuỗi điều hòa), n rất lớn (tới 1e12+).
// ĐPT: mọi hàm O(sqrt n) (khoảng 2*sqrt(n) khối); bộ nhớ O(1), riêng floorBlocks O(sqrt n) để chứa kết quả.
// Dùng: sumFloorDiv(10);            // 27 = tổng số ước của 1..10
//       sumDivisorsAll(10);         // 87 = tổng các ước của 1..10 (thêm tham số mod nếu cần: sumDivisorsAll(n, 1000000007))
//       for (auto [l, r] : floorBlocks(n)) { long long q = n / l; /* floor(n/i) == q với mọi i trong [l, r] */ }
// Bẫy: i chạy 1-based, khối [l, r] là đoạn ĐÓNG và phủ kín [1, n] không chồng lấn; n <= 0 trả về 0 / rỗng.
//      sumDivisorsAll KHÔNG mod chỉ đúng khi n <= ~3.3e9 (kết quả ~0.822*n^2, quá đó tràn long long) — dùng mod cho n lớn.
//      Với mod, hàm dùng __int128 nội bộ nên mod tới 9e18 vẫn an toàn; mod phải > 0 mới bật chế độ lấy dư.
// CSES: 1082
// Ý nghĩa: liệt kê các khối [l, r] mà floor(n/i) bằng nhau với mọi i thuộc [l, r] (dùng để tự viết công thức tổng).
// Tham số: n = giới hạn trên của i (i chạy 1..n).
// Trả về: danh sách cặp (l, r) tăng dần, phủ kín [1, n] và không chồng lấn; rỗng nếu n <= 0.
vector<pair<long long, long long>> floorBlocks(long long n) {
    vector<pair<long long, long long>> res;
    if (n <= 0) return res;
    for (long long l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);   // r = chỉ số lớn nhất còn giữ nguyên thương floor(n/l)
        res.push_back({l, r});
    }
    return res;
}
// Ý nghĩa: tính sum_{i=1..n} floor(n/i) — cũng bằng TỔNG SỐ ƯỚC của tất cả các số trong [1, n].
// Tham số: n = giới hạn trên.
// Trả về: giá trị tổng (0 nếu n <= 0); tổng cỡ n*ln(n) nên long long đủ tới n ~ 1e17.
long long sumFloorDiv(long long n) {
    if (n <= 0) return 0;
    long long res = 0;
    for (long long l = 1, r; l <= n; l = r + 1) {
        long long q = n / l;
        r = n / q;
        res += q * (r - l + 1);   // cả khối [l, r] có cùng thương q
    }
    return res;
}
// Ý nghĩa: tính sum_{i=1..n} i*floor(n/i) — bằng TỔNG CÁC ƯỚC của tất cả các số trong [1, n] (CSES 1082).
// Tham số: n = giới hạn trên; mod = modulo (mod > 0 thì lấy dư, mod <= 0 thì tính chính xác không lấy dư).
// Trả về: giá trị tổng, đã lấy dư theo mod nếu mod > 0 (0 nếu n <= 0).
long long sumDivisorsAll(long long n, long long mod = 0) {
    if (n <= 0) return 0;
    __int128 res = 0;
    for (long long l = 1, r; l <= n; l = r + 1) {
        long long q = n / l;
        r = n / q;
        // Tổng các i trong khối: (l + r) * (r - l + 1) / 2, tính bằng __int128 để tránh tràn khi n ~ 1e12.
        __int128 blockSum = ((__int128)(l + r) * (__int128)(r - l + 1)) / 2;
        if (mod > 0) res = (res + (__int128)(long long)(blockSum % mod) * (long long)(q % mod)) % mod;
        else res += blockSum * (__int128)q;
    }
    return (long long)res;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ divisor-blocks.hpp -o demo && ./demo
int main() {
    printf("sumFloorDiv(10)=%lld sumDivisorsAll(10)=%lld\n",
           sumFloorDiv(10), sumDivisorsAll(10));                       // 27 87
    printf("số khối của n=10:%d ->", (int)floorBlocks(10).size());     // 5
    for (auto [l, r] : floorBlocks(10)) printf(" [%lld,%lld]q=%lld", l, r, 10 / l);
    printf("\n");                                                      // [1,1]q=10 [2,2]q=5 [3,3]q=3 [4,5]q=2 [6,10]q=1
    long long n = 1000000000000LL;                                     // CSES 1082 với n = 1e12
    printf("sumDivisorsAll(1e12) mod 1e9+7 = %lld ; sumFloorDiv(1e12) = %lld\n",
           sumDivisorsAll(n, 1000000007LL), sumFloorDiv(n));           // 146462362 ; 27785452449086
    return 0;
}
#endif
