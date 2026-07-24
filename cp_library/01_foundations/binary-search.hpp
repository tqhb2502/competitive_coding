#pragma once
#include <bits/stdc++.h>
using namespace std;
// Binary Search on Answer — tìm biên của một vị từ ĐƠN ĐIỆU trên số nguyên.
// Khi dùng: pred(x) có dạng F..F,T..T (hoặc T..T,F..F) trên [lo, hi].
// ĐPT: O(log(hi - lo)) lần gọi pred.
// Dùng: long long x = firstTrue(lo, hi, [&](long long m){ return check(m); });
//        firstTrue: x nhỏ nhất pred đúng (trả hi+1 nếu không có).
//        lastTrue : x lớn nhất pred đúng (trả lo-1 nếu không có).
// Bẫy: pred phải đơn điệu; mid = lo + (hi-lo)/2 để tránh tràn; cả lo và hi đều được xét.
// CSES: 1073 1085 1086 1140 1145 1162 1620 1623 1628 1664 2084 2422 3112 3273 3301 3305 3401 3404
template <class F>
long long firstTrue(long long lo, long long hi, F pred) {  // pred: F..F,T..T
    long long res = hi + 1;
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        if (pred(mid)) { res = mid; hi = mid - 1; }
        else lo = mid + 1;
    }
    return res;
}
template <class F>
long long lastTrue(long long lo, long long hi, F pred) {  // pred: T..T,F..F
    long long res = lo - 1;
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        if (pred(mid)) { res = mid; lo = mid + 1; }
        else hi = mid - 1;
    }
    return res;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ binary-search.hpp -o demo && ./demo
int main() {
    long long x = firstTrue(0, 100, [](long long m) { return m * m >= 50; });
    printf("x nho nhat co x*x>=50: %lld\n", x);   // 8
    return 0;
}
#endif
