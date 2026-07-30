#pragma once
#include <bits/stdc++.h>
using namespace std;
// K-best Combinations — liệt kê k tổng nhỏ nhất của tập con có đúng m phần tử
// bằng heap trên cây phân hoạch, không sinh trùng tổ hợp chỉ số.
// Khi dùng: n lớn, không thể duyệt C(n,m), nhưng chỉ cần một số ít tổng nhỏ nhất.
// Giá trị được phép âm/trùng; hai tập chỉ số khác nhau luôn được tính riêng.
// ĐPT: O(n log n + r log r), bộ nhớ O(n+r), r=min(k,C(n,m)).
// Dùng: auto ans = kSmallestFixedSizeSubsetSums(a, m, k);
// Bẫy: m phải thuộc [0,n], k >= 0; kết quả có thể ngắn hơn k nếu k>C(n,m).
// Hàm ném overflow_error chỉ khi một trong r tổng cần trả không vừa long long.
// CSES: 3109

namespace k_best_combinations_detail {

struct State {
    __int128 sum;
    int parts;       // số phần dương của phân hoạch
    int last;        // phần cuối
    int previous;    // phần áp cuối
};

struct StateGreater {
    bool operator()(const State& first, const State& second) const {
        if (first.sum != second.sum) return first.sum > second.sum;
        if (first.parts != second.parts) return first.parts > second.parts;
        if (first.last != second.last) return first.last > second.last;
        return first.previous > second.previous;
    }
};

inline long long checkedLongLong(__int128 value) {
    if (value < numeric_limits<long long>::min() ||
        value > numeric_limits<long long>::max())
        throw overflow_error(
            "kSmallestFixedSizeSubsetSums: tổng kết quả tràn long long");
    return static_cast<long long>(value);
}

}  // namespace k_best_combinations_detail

// Ý nghĩa: lấy k tổng nhỏ nhất trong multiset tổng của mọi tập con đúng m phần tử.
// Tham số: values = dãy có dấu; subsetSize = m; k = số kết quả yêu cầu.
// Trả về: min(k,C(n,m)) tổng không giảm, giữ đúng multiplicity theo tập chỉ số.
inline vector<long long> kSmallestFixedSizeSubsetSums(
        const vector<long long>& values, int subsetSize, long long k) {
    if (values.size() > static_cast<size_t>(numeric_limits<int>::max()))
        throw length_error(
            "kSmallestFixedSizeSubsetSums: n vượt miền chỉ số int");
    if (subsetSize < 0 || subsetSize > static_cast<int>(values.size()))
        throw invalid_argument(
            "kSmallestFixedSizeSubsetSums: subsetSize ngoài [0,n]");
    if (k < 0)
        throw invalid_argument("kSmallestFixedSizeSubsetSums: k âm");
    if (k == 0) return {};

    vector<long long> sortedValues = values;
    sort(sortedValues.begin(), sortedValues.end());
    __int128 initialSum = 0;
    for (int index = 0; index < subsetSize; ++index)
        initialSum += sortedValues[index];

    using k_best_combinations_detail::State;
    priority_queue<State, vector<State>,
                   k_best_combinations_detail::StateGreater> queue;
    queue.push({initialSum, 0, 0, 0});
    vector<long long> answer;

    while (!queue.empty() && static_cast<long long>(answer.size()) < k) {
        const State current = queue.top();
        queue.pop();
        answer.push_back(
            k_best_combinations_detail::checkedLongLong(current.sum));

        // Nối một phần 1: dịch phần tử ngay trước suffix đang dịch sang phải.
        // Điều kiện index+1<n xử lý đúng trường hợp m=n (chỉ có tập toàn bộ).
        if (current.parts < subsetSize) {
            const int index = subsetSize - current.parts - 1;
            if (index + 1 < static_cast<int>(sortedValues.size())) {
                const __int128 childSum =
                    current.sum +
                    static_cast<__int128>(sortedValues[index + 1]) -
                    sortedValues[index];
                queue.push(
                    {childSum, current.parts + 1, 1, current.last});
            }
        }

        // Tăng phần cuối nếu vẫn là một phân hoạch và chỉ số mới còn trong mảng.
        if (current.parts > 0 &&
            (current.parts == 1 || current.last < current.previous)) {
            const int baseIndex = subsetSize - current.parts;
            const int valueIndex = baseIndex + current.last;
            if (valueIndex + 1 < static_cast<int>(sortedValues.size())) {
                const __int128 childSum =
                    current.sum +
                    static_cast<__int128>(sortedValues[valueIndex + 1]) -
                    sortedValues[valueIndex];
                queue.push({childSum, current.parts, current.last + 1,
                            current.previous});
            }
        }
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ k-best-combinations.hpp -o demo && ./demo
int main() {
    auto answer = kSmallestFixedSizeSubsetSums({-4, 1, 1, 7}, 2, 8);
    printf("Các tổng nhỏ nhất của tập con 2 phần tử:");
    for (long long value : answer)
        printf(" %lld", value); // -3 -3 2 3 8 8 (chỉ có C(4,2)=6 tập)
    printf("\n");
    return 0;
}
#endif
