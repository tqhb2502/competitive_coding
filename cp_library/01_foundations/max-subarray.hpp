#pragma once
#include <bits/stdc++.h>
using namespace std;
// Kadane (Maximum Subarray) — tổng LỚN NHẤT của một đoạn con LIỀN KỀ khác rỗng, kèm biến thể trả cả biên.
// Khi dùng: "đoạn con liên tiếp có tổng lớn nhất", lãi/lỗ tối đa của một khoảng ngày liên tục.
// ĐPT: O(n) một lượt quét, không tiền xử lý; bộ nhớ O(1) ngoài mảng đầu vào.
// Dùng: long long s = maxSubarraySum(a);
//        auto [s, l, r] = maxSubarrayRange(a);   // đoạn NỬA MỞ [l, r): a[l], ..., a[r-1]
// Bẫy: đoạn KHÔNG RỖNG — mọi phần tử âm thì đáp án là phần tử LỚN NHẤT (âm), tuyệt đối không trả 0;
//      mảng 0-based, biên trả về là nửa mở [l, r) nên số phần tử = r - l >= 1;
//      tổng có thể tới n * max|a[i]| nên luôn dùng long long (mảng vào cũng là long long);
//      mảng rỗng là đầu vào KHÔNG hợp lệ (không có đoạn khác rỗng) — hàm trả LLONG_MIN với l = r = 0;
//      nếu nhiều đoạn cùng tổng lớn nhất, hàm trả đoạn có r NHỎ NHẤT (không đảm bảo ngắn nhất).
// CSES: 1643
// Ý nghĩa: tính tổng lớn nhất trên mọi đoạn con liền kề khác rỗng của a (thuật toán Kadane).
// Tham số: a = mảng đầu vào 0-based, nên khác rỗng, cho phép giá trị âm.
// Trả về: tổng lớn nhất; nếu a rỗng trả LLONG_MIN (quy ước "không có đoạn hợp lệ").
inline long long maxSubarraySum(const vector<long long>& a) {
    if (a.empty()) return LLONG_MIN;
    long long best = a[0], cur = a[0];
    for (size_t i = 1; i < a.size(); i++) {
        // cur = tổng lớn nhất của đoạn KẾT THÚC tại i: hoặc nối tiếp đoạn trước, hoặc bắt đầu lại tại i.
        cur = max(a[i], cur + a[i]);
        best = max(best, cur);
    }
    return best;
}
// Ý nghĩa: như maxSubarraySum nhưng truy vết thêm biên của một đoạn đạt tổng lớn nhất.
// Tham số: a = mảng đầu vào 0-based, nên khác rỗng, cho phép giá trị âm.
// Trả về: bộ ba (tổng lớn nhất, l, r) với đoạn NỬA MỞ [l, r) tức a[l..r-1]; nếu a rỗng trả (LLONG_MIN, 0, 0).
inline tuple<long long, int, int> maxSubarrayRange(const vector<long long>& a) {
    if (a.empty()) return {LLONG_MIN, 0, 0};
    int n = (int)a.size();
    long long best = a[0], cur = a[0];
    int bestL = 0, bestR = 1, curL = 0;   // [bestL, bestR) là đoạn tốt nhất; curL là đầu đoạn đang chạy
    for (int i = 1; i < n; i++) {
        if (cur >= 0) {
            cur += a[i];                  // nối tiếp đoạn cũ vẫn có lợi (hoặc không lỗ)
        } else {
            cur = a[i]; curL = i;         // đoạn cũ đang âm, bỏ hẳn và bắt đầu lại tại i
        }
        if (cur > best) { best = cur; bestL = curL; bestR = i + 1; }
    }
    return {best, bestL, bestR};
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ max-subarray.hpp -o demo && ./demo
int main() {
    vector<long long> a = {-1, 3, -2, 5, 3, -5, 2, 2};
    auto [s, l, r] = maxSubarrayRange(a);
    printf("sum=%lld range=[%d,%d)\n", s, l, r);              // sum=9 range=[1,5)
    printf("maxSubarraySum=%lld\n", maxSubarraySum(a));        // maxSubarraySum=9
    vector<long long> neg = {-5, -2, -9};                      // toàn âm -> đáp án là phần tử lớn nhất
    auto [s2, l2, r2] = maxSubarrayRange(neg);
    printf("allNeg sum=%lld range=[%d,%d)\n", s2, l2, r2);    // allNeg sum=-2 range=[1,2)
    return 0;
}
#endif
