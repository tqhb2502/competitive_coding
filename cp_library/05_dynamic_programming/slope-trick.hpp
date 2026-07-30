#pragma once
#include <bits/stdc++.h>
using namespace std;
// Slope trick (hồi quy đẳng điệu L1 / PAVA) — chi phí nhỏ nhất sửa dãy thành KHÔNG GIẢM với giá |a_i - a'_i|.
// Khi dùng: "mỗi thao tác ±1 một phần tử, tổng thao tác nhỏ nhất để dãy không giảm (hoặc tăng ngặt)".
// ĐPT: minCostNonDecreasing O(n log n) (mỗi phần tử 1 push + tối đa 1 pop/push); minCostIncreasing O(n log n) (thêm O(n) dựng a_i - i); bộ nhớ O(n).
// Dùng: long long c1 = minCostNonDecreasing(a); long long c2 = minCostIncreasing(a);   // a là vector<long long>, 0-based
// Bẫy: chi phí tới ~2e14 nên PHẢI long long; chỉ trả CHI PHÍ (không dựng lại dãy tối ưu); minCostIncreasing giả định dãy đích NGUYÊN (bước >= 1) và cho phép a'_i âm.
// CSES: 2132
// Ý nghĩa: chi phí nhỏ nhất (tổng |a_i - a'_i|) để biến a thành dãy KHÔNG GIẢM a'; mỗi lần ±1 một phần tử tốn 1.
//          Max-heap giữ các "mức" mô tả hàm chi phí lồi của tiền tố: đỉnh heap là điểm cực tiểu lớn nhất hiện tại.
// Tham số: a = dãy đầu vào (0-based; giá trị bất kỳ, kể cả âm; a rỗng trả 0).
// Trả về: tổng chi phí nhỏ nhất.
long long minCostNonDecreasing(const vector<long long>& a) {
    priority_queue<long long> levels;   // các mức của nghiệm tối ưu trên tiền tố (trung vị từng khối PAVA)
    long long cost = 0;
    for (long long x : a) {
        levels.push(x);
        if (levels.top() > x) {         // khối cuối vi phạm thứ tự: kéo mức lớn nhất xuống x
            cost += levels.top() - x;
            levels.pop();
            levels.push(x);
        }
    }
    return cost;
}

// Ý nghĩa: chi phí nhỏ nhất để biến a thành dãy TĂNG NGẶT (a'_i > a'_{i-1}, mọi a'_i nguyên).
//          Đặt b_i = a_i - i thì "a' tăng ngặt" tương đương "b' không giảm", chi phí giữ nguyên.
// Tham số: a = dãy đầu vào (0-based; a rỗng trả 0).
// Trả về: tổng chi phí nhỏ nhất.
long long minCostIncreasing(const vector<long long>& a) {
    vector<long long> b(a.size());
    for (size_t i = 0; i < a.size(); i++) b[i] = a[i] - (long long)i;
    return minCostNonDecreasing(b);
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ slope-trick.hpp -o demo && ./demo
int main() {
    vector<long long> a = {3, 2, 5, 1, 7};
    printf("chi phí -> không giảm = %lld\n", minCostNonDecreasing(a));   // 5  (3 2 5 1 7 -> 2 2 2 2 7)
    printf("chi phí -> tăng ngặt  = %lld\n", minCostIncreasing(a));      // 7  (3 2 5 1 7 -> 1 2 3 4 7)

    vector<long long> sorted_already = {-4, -4, 0, 9};
    printf("dãy đã không giảm     = %lld\n", minCostNonDecreasing(sorted_already));   // 0
    printf("dãy đã không giảm (ngặt) = %lld\n", minCostIncreasing(sorted_already));   // 1  (-4 -4 0 9 -> -5 -4 0 9)

    vector<long long> empty_seq;
    printf("dãy rỗng              = %lld\n", minCostNonDecreasing(empty_seq));   // 0
    return 0;
}
#endif
