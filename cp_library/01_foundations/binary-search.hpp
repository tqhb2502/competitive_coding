#pragma once
#include <bits/stdc++.h>
using namespace std;
// Binary Search on Answer — tìm biên của một vị từ ĐƠN ĐIỆU trên số nguyên.
// Khi dùng: pred(x) có dạng F..F,T..T (hoặc T..T,F..F) trên [lo, hi].
// ĐPT: O(log(hi - lo)) lần gọi pred.
// Dùng: long long x = firstTrue(lo, hi, [&](long long m){ return check(m); });
//        // firstTrue: x nhỏ nhất pred đúng (trả hi+1 nếu không có).
//        // lastTrue : x lớn nhất pred đúng (trả lo-1 nếu không có).
// Bẫy: pred phải đơn điệu; mid = lo + (hi-lo)/2 để tránh tràn; cả lo và hi đều được xét.
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
