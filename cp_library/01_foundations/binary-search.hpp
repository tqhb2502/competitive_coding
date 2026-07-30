#pragma once
#include <bits/stdc++.h>
using namespace std;
// Binary Search — lower/upper bound trên dãy ĐÃ SORT và tìm biên của vị từ đơn điệu trên số nguyên.
// Khi dùng: tìm vị trí chèn/rank trong sorted array; hoặc pred dạng F..F,T..T (hay T..T,F..F) trên [lo,hi].
// ĐPT: O(log n) cho sorted search; O(log(hi-lo+1)) lần gọi pred; bộ nhớ O(1).
// Dùng: lowerBoundIndex(a,x); firstTrue(lo,hi,[&](long long m){return check(m);}); lastTrue(...);
// Bẫy: mảng phải sort theo <; pred phải đơn điệu; sentinel hi+1/lo-1 không biểu diễn được ở biên LLONG thì ném overflow_error.
// CSES: 1073 1085 1086 1140 1145 1162 1620 1623 1628 1664 2084 2422 3112 3273 3301 3305 3401 3404 3425
// Ý nghĩa: tìm x nhỏ nhất trong [lo, hi] mà pred(x) đúng, với pred đơn điệu dạng F..F,T..T.
// Tham số: lo, hi = biên trái/phải của khoảng tìm (cả hai đều được xét); pred = vị từ đơn điệu.
// Trả về: x nhỏ nhất pred đúng; trả hi+1 nếu không tồn tại.
template <class F>
long long firstTrue(long long lo, long long hi, F pred) {  // pred: F..F,T..T
    long long originalHi = hi, res = 0;
    bool found = false;
    while (lo <= hi) {
        long long mid = (long long)((__int128)lo + ((__int128)hi - lo) / 2);
        if (pred(mid)) {
            found = true;
            res = mid;
            if (mid == LLONG_MIN) break;
            hi = mid - 1;
        }
        else {
            if (mid == LLONG_MAX) break;
            lo = mid + 1;
        }
    }
    if (found) return res;
    if (originalHi == LLONG_MAX) throw overflow_error("firstTrue: sentinel hi+1 bị tràn");
    return originalHi + 1;
}
// Ý nghĩa: tìm x lớn nhất trong [lo, hi] mà pred(x) đúng, với pred đơn điệu dạng T..T,F..F.
// Tham số: lo, hi = biên trái/phải của khoảng tìm (cả hai đều được xét); pred = vị từ đơn điệu.
// Trả về: x lớn nhất pred đúng; trả lo-1 nếu không tồn tại.
template <class F>
long long lastTrue(long long lo, long long hi, F pred) {  // pred: T..T,F..F
    long long originalLo = lo, res = 0;
    bool found = false;
    while (lo <= hi) {
        long long mid = (long long)((__int128)lo + ((__int128)hi - lo) / 2);
        if (pred(mid)) {
            found = true;
            res = mid;
            if (mid == LLONG_MAX) break;
            lo = mid + 1;
        }
        else {
            if (mid == LLONG_MIN) break;
            hi = mid - 1;
        }
    }
    if (found) return res;
    if (originalLo == LLONG_MIN) throw overflow_error("lastTrue: sentinel lo-1 bị tràn");
    return originalLo - 1;
}

// Ý nghĩa: vị trí đầu tiên i sao cho sorted[i] >= value (giống std::lower_bound).
// Tham số: sorted = dãy tăng không giảm; value = khóa cần tìm.
// Trả về: chỉ số 0-based trong [0,n], bằng n nếu mọi phần tử < value.
template <class T, class U>
int lowerBoundIndex(const vector<T>& sorted, const U& value) {
    int lo = 0, hi = (int)sorted.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (sorted[mid] < value) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// Ý nghĩa: vị trí đầu tiên i sao cho sorted[i] > value (giống std::upper_bound).
// Tham số: sorted = dãy tăng không giảm; value = khóa cần tìm.
// Trả về: chỉ số 0-based trong [0,n], bằng n nếu mọi phần tử <= value.
template <class T, class U>
int upperBoundIndex(const vector<T>& sorted, const U& value) {
    int lo = 0, hi = (int)sorted.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (!(value < sorted[mid])) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ binary-search.hpp -o demo && ./demo
int main() {
    long long x = firstTrue(0, 100, [](long long m) { return m * m >= 50; });
    vector<int> sorted = {1, 2, 2, 5};
    printf("x nhỏ nhất có x*x>=50: %lld; equal-range(2)=[%d,%d)\n",
           x, lowerBoundIndex(sorted, 2), upperBoundIndex(sorted, 2));   // 8; [1,3)
    return 0;
}
#endif
