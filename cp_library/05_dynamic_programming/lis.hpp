#pragma once
#include <bits/stdc++.h>
using namespace std;
// LIS — độ dài dãy con TĂNG NGẶT dài nhất, O(n log n) bằng mảng "đuôi nhỏ nhất" + binary search.
// Khi dùng: tìm dãy con tăng dài nhất; nền cho nhiều bài (xếp hộp, lịch, phủ...).
// ĐPT: O(n log n); bộ nhớ O(n). Mảng bất kỳ; kết quả là ĐỘ DÀI (không phải chính dãy con).
// Dùng: int L = lisLength(a); // dãy tăng NGẶT; đổi lower_bound -> upper_bound để cho phép KHÔNG giảm (tăng không ngặt)
// Bẫy: "ngặt" dùng lower_bound (thay phần tử bằng nhau); "không ngặt" dùng upper_bound; chỉ trả độ dài.
// CSES: 1145 1162 3404
// Ý nghĩa: tính độ dài dãy con tăng ngặt dài nhất của a bằng kỹ thuật mảng đuôi + binary search.
// Tham số: a = mảng đầu vào.
// Trả về: độ dài LIS (tăng ngặt).
int lisLength(const vector<int>& a) {
    vector<int> tail;   // tail[i] = đuôi NHỎ NHẤT của dãy tăng độ dài i+1
    for (int x : a) {
        auto it = lower_bound(tail.begin(), tail.end(), x);
        if (it == tail.end()) tail.push_back(x);
        else *it = x;
    }
    return (int)tail.size();
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ lis.hpp -o demo && ./demo
int main() {
    vector<int> a = {7, 3, 5, 3, 6, 2, 9, 8};
    printf("độ dài LIS (tăng ngặt) = %d\n", lisLength(a));   // 3 5 6 9 -> 4
    return 0;
}
#endif
