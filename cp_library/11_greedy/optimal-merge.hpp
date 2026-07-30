#pragma once
#include <bits/stdc++.h>
using namespace std;
// Optimal Merge (Huffman) — luôn ghép hai trọng số nhỏ nhất để tối thiểu tổng chi phí ghép.
// Khi dùng: cắt thanh đảo ngược thành ghép; nối file/rope; xây cây prefix khi mỗi lần ghép tốn tổng hai phần.
// ĐPT: O(n log n); bộ nhớ O(n).
// Dùng: long long cost = optimalMergeCost(lengths);
// Bẫy: trọng số phải không âm; 0/1 phần tử có chi phí 0; n lớn cần long long và kiểm tràn.
// CSES: 1161
// Ý nghĩa: tính chi phí nhỏ nhất để ghép mọi trọng số thành một khối.
// Tham số: weights = trọng số lá không âm.
// Trả về: tổng chi phí tối ưu; ném overflow_error nếu vượt long long.
long long optimalMergeCost(const vector<long long>& weights) {
    priority_queue<long long, vector<long long>, greater<long long>> heap;
    for (long long weight : weights) {
        if (weight < 0) throw invalid_argument("optimalMergeCost: trọng số âm");
        heap.push(weight);
    }
    long long answer = 0;
    while (heap.size() > 1) {
        long long a = heap.top(); heap.pop();
        long long b = heap.top(); heap.pop();
        if (a > LLONG_MAX - b) throw overflow_error("optimalMergeCost: tràn lần ghép");
        long long merged = a + b;
        if (answer > LLONG_MAX - merged) throw overflow_error("optimalMergeCost: tràn đáp án");
        answer += merged;
        heap.push(merged);
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ optimal-merge.hpp -o demo && ./demo
int main() {
    printf("optimal merge cost = %lld\n", optimalMergeCost({2, 3, 4, 6}));  // 29
    return 0;
}
#endif
