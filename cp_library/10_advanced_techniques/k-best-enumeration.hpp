#pragma once
#include <bits/stdc++.h>
using namespace std;
// K-best Enumeration — liệt kê k tổng tập con nhỏ nhất bằng heap trên cây trạng thái không trùng.
// Khi dùng: n lớn nhưng chỉ cần k tổng tập con nhỏ nhất; phần tử có thể âm; tính cả multiplicity theo tập chỉ số.
// ĐPT: O(n log n + k log k); bộ nhớ O(n+k).
// Dùng: auto sums = kSmallestSubsetSums(values, k); // tăng dần, chứa tổng tập rỗng
// Bẫy: các tổng bằng nhau được lặp theo các tập chỉ số khác nhau; có tối đa 2^n kết quả; tổng phải vừa long long.
// CSES: 3108
// Ý nghĩa: lấy k tổng nhỏ nhất trong multiset tổng của mọi tập con.
// Tham số: values = dãy số có dấu; k = số kết quả cần (k>=0).
// Trả về: min(k,2^n) tổng theo thứ tự không giảm, tính cả tập rỗng.
vector<long long> kSmallestSubsetSums(const vector<long long>& values, long long k) {
    if (k < 0) throw invalid_argument("kSmallestSubsetSums: k âm");
    vector<long long> cost;
    cost.reserve(values.size());
    long long base = 0;
    for (long long value : values) {
        if (value < 0) {
            if (base < LLONG_MIN - value) throw overflow_error("kSmallestSubsetSums: tràn base");
            base += value;
            if (value == LLONG_MIN) throw overflow_error("kSmallestSubsetSums: abs(LLONG_MIN)");
            cost.push_back(-value);
        } else cost.push_back(value);
    }
    sort(cost.begin(), cost.end());
    vector<long long> answer;
    if (k == 0) return answer;
    answer.push_back(base);
    if (cost.empty()) return answer;
    using State = pair<long long, int>;  // {phần cộng không âm, chỉ số lớn nhất đang dùng}
    priority_queue<State, vector<State>, greater<State>> heap;
    heap.push({cost[0], 0});
    while (!heap.empty() && (long long)answer.size() < k) {
        auto [sum, index] = heap.top();
        heap.pop();
        if ((sum > 0 && base > LLONG_MAX - sum) || (sum < 0 && base < LLONG_MIN - sum))
            throw overflow_error("kSmallestSubsetSums: tràn output");
        answer.push_back(base + sum);
        if (index + 1 == (int)cost.size()) continue;
        long long next = cost[index + 1];
        if (sum > LLONG_MAX - next) throw overflow_error("kSmallestSubsetSums: tràn trạng thái");
        heap.push({sum + next, index + 1});
        // sum >= cost[index], nên phép thay không âm và không tràn dưới.
        if (sum - cost[index] > LLONG_MAX - next)
            throw overflow_error("kSmallestSubsetSums: tràn trạng thái thay");
        heap.push({sum - cost[index] + next, index + 1});
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ k-best-enumeration.hpp -o demo && ./demo
int main() {
    auto answer = kSmallestSubsetSums({-3, 1, 4}, 6);
    printf("6 tổng tập con nhỏ nhất:");
    for (long long value : answer) printf(" %lld", value);  // -3 -2 0 1 1 2
    printf("\n");
    return 0;
}
#endif
