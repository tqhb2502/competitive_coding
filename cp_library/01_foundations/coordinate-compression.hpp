#pragma once
#include <bits/stdc++.h>
using namespace std;
// Coordinate Compression — nén giá trị lớn về hạng 0..k-1, giữ thứ tự.
// Khi dùng: chỉ cần thứ tự tương đối để làm chỉ số cho Fenwick/segment tree...
// ĐPT: build O(n log n); id/val O(log n); bộ nhớ O(n).
// Dùng: Compress cc(vals); int r = cc.id(x); long long g = cc.val(r); int k = cc.size();
//        id(x) = số phần tử phân biệt < x = hạng 0-based (dùng lower_bound).
// Bẫy: id trả hạng theo lower_bound nên với x không có trong tập vẫn cho vị trí chèn.
// CSES: 1091 1144 1163 1203 1632 1664 1740 1741 1742 1748 2169 2184 2428 3150 3163 3190 3223 3224 3306 3307 3356 3401
struct Compress {
    vector<long long> v;
    // Ý nghĩa: dựng bảng nén — sao chép a, sắp xếp tăng và loại trùng để v là tập giá trị phân biệt tăng dần.
    // Tham số: a = mảng giá trị gốc cần nén (kiểu T bất kỳ chuyển được sang long long).
    template <class T>
    explicit Compress(vector<T> a) : v(a.begin(), a.end()) {
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
    }
    // Ý nghĩa: số lượng giá trị phân biệt sau khi nén (kích thước miền hạng 0..k-1).
    // Trả về: k = số phần tử của v.
    int size() const { return (int)v.size(); }
    // Ý nghĩa: hạng 0-based của x = số phần tử phân biệt nhỏ hơn x (theo lower_bound).
    // Tham số: x = giá trị cần tra hạng (không cần tồn tại trong tập).
    // Trả về: chỉ số chèn của x trong v; nếu x có mặt thì là hạng của nó.
    int id(long long x) const {
        return (int)(lower_bound(v.begin(), v.end(), x) - v.begin());
    }
    // Ý nghĩa: giá trị gốc ứng với hạng r (phép nghịch của id đối với giá trị có mặt).
    // Tham số: r = hạng 0-based trong [0, size()).
    // Trả về: v[r] = giá trị phân biệt thứ r theo thứ tự tăng dần.
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
