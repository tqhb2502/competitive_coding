#pragma once
#include <bits/stdc++.h>
using namespace std;
// Coordinate Compression — nén giá trị lớn về hạng 0..k-1, giữ thứ tự.
// Khi dùng: chỉ cần thứ tự tương đối để làm chỉ số cho Fenwick/segment tree...
// ĐPT: build O(n log n); id/val O(log n); bộ nhớ O(n).
// Dùng: Compress cc(vals); int r = cc.id(x); long long g = cc.val(r); int k = cc.size();
//        // id(x) = số phần tử phân biệt < x = hạng 0-based (dùng lower_bound).
// Bẫy: id trả hạng theo lower_bound nên với x không có trong tập vẫn cho vị trí chèn.
// CSES: 1091 1144 1163 1203 1664 1740 1741 1742 1748 2169 2184 2428 3150 3163 3190 3223 3224 3306 3307 3356 3401
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

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ coordinate-compression.hpp -o demo && ./demo
int main() {
    vector<int> v = {100, 5, 100, 42};
    Compress cc(v);
    printf("size=%d  id(42)=%d  val(1)=%lld\n", cc.size(), cc.id(42), cc.val(1));
    return 0;
}
#endif
