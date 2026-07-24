#pragma once
#include <bits/stdc++.h>
using namespace std;
// Two Pointers — hai con trỏ tiến một chiều trên mảng ĐÃ SẮP XẾP.
// Khi dùng: tìm cặp theo tổng, đếm cặp thỏa điều kiện đơn điệu theo tổng.
// ĐPT: O(n) sau khi mảng đã sort.
// Dùng: auto p = twoSumSorted(a, target);      // {i, j} với a[i]+a[j]==target, hoặc {-1,-1}
//        long long c = countPairsSumAtMost(a, K); // số cặp i<j có a[i]+a[j] <= K
// Bẫy: input PHẢI đã sort tăng dần; chỉ số trả về theo mảng đã sort; đếm dùng long long.
// CSES: 1084 1090 1141 1191 1619 1641 1660 2087 2428 3190 3306 3307 3355 3416 3420 3425 3428
template <class T>
pair<int, int> twoSumSorted(const vector<T>& a, T target) {
    int i = 0, j = (int)a.size() - 1;
    while (i < j) {
        T s = a[i] + a[j];
        if (s == target) return {i, j};
        if (s < target) i++; else j--;
    }
    return {-1, -1};
}
template <class T>
long long countPairsSumAtMost(const vector<T>& a, T K) {  // a đã sort tăng dần
    long long c = 0;
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        if (a[l] + a[r] <= K) { c += r - l; l++; }  // mọi x trong (l, r] đều thỏa
        else r--;
    }
    return c;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ two-pointers.hpp -o demo && ./demo
int main() {
    vector<int> a = {1, 2, 3, 4, 6};   // da sort tang dan
    auto p = twoSumSorted(a, 7);
    printf("cap tong 7 -> (%d,%d);  so cap tong<=5: %lld\n",
           p.first, p.second, countPairsSumAtMost(a, 5));
    return 0;
}
#endif
