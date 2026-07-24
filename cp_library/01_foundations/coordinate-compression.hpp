#pragma once
#include <bits/stdc++.h>
using namespace std;
// Coordinate Compression — nén giá trị lớn về hạng 0..k-1, giữ thứ tự.
// Khi dùng: chỉ cần thứ tự tương đối để làm chỉ số cho Fenwick/segment tree...
// ĐPT: build O(n log n); id/val O(log n); bộ nhớ O(n).
// Dùng: Compress cc(vals); int r = cc.id(x); long long g = cc.val(r); int k = cc.size();
//        // id(x) = số phần tử phân biệt < x = hạng 0-based (dùng lower_bound).
// Bẫy: id trả hạng theo lower_bound nên với x không có trong tập vẫn cho vị trí chèn.
struct Compress {
    vector<long long> v;
    template <class T>
    explicit Compress(vector<T> a) : v(a.begin(), a.end()) {
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
    }
    int size() const { return (int)v.size(); }
    int id(long long x) const {
        return (int)(lower_bound(v.begin(), v.end(), x) - v.begin());
    }
    long long val(int r) const { return v[r]; }
};
