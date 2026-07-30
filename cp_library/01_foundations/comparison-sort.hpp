#pragma once
#include <bits/stdc++.h>
using namespace std;
// Comparison Sort — merge sort ổn định khi phép so sánh là một oracle đắt tiền, đồng thời đếm chính xác số lần gọi oracle.
// Khi dùng: chỉ biết thứ tự qua callback/query; cần giữ thứ tự tương đối của phần tử tương đương; cần chứng minh không vượt query budget.
// ĐPT: stableOracleMergeSort O(n log n) phép gán và tối đa n*ceil(log2(n))-2^ceil(log2(n))+1 phép so sánh; bộ nhớ O(n).
// Dùng: unsigned long long q = stableOracleMergeSort(ids, [&](int i, int j) { return ask(i, j); });
//        auto worst = oracleMergeSortWorstComparisons(ids.size());  // kiểm tra trước query budget
// Bẫy: oracleLess(a,b) phải là strict-weak-order và KHÔNG được thay đổi thứ tự ẩn; merge gọi oracleLess(phải,trái) để ưu tiên bản trái khi tương đương.
//       Hàm sắp xếp tại chỗ; nếu callback ném exception thì vector có thể mới được sắp một phần. Công thức worst-case ứng với đúng cách chia đôi top-down bên dưới.
// CSES: 3139 3151

// Ý nghĩa: tính số phép so sánh lớn nhất của merge sort top-down cân bằng dùng trong stableOracleMergeSort.
// Tham số: n = số phần tử.
// Trả về: 0 nếu n <= 1, ngược lại n*ceil(log2(n))-2^ceil(log2(n))+1; ném overflow_error nếu kết quả không vừa unsigned long long.
inline unsigned long long oracleMergeSortWorstComparisons(size_t n) {
    if (n <= 1) return 0;
    int levels = 0;
    unsigned __int128 power = 1;
    while (power < n) {
        power <<= 1;
        ++levels;
    }
    unsigned __int128 answer = (unsigned __int128)n * levels - power + 1;
    if (answer > numeric_limits<unsigned long long>::max())
        throw overflow_error("oracleMergeSortWorstComparisons: kết quả vượt unsigned long long");
    return (unsigned long long)answer;
}

// Ý nghĩa: sắp xếp ổn định items tại chỗ bằng merge sort và đếm đúng số lần gọi oracleLess.
// Tham số: items = vector cần sắp; oracleLess(a,b) = true khi a phải đứng trước b, tuân theo strict-weak-order.
// Trả về: số lần oracleLess thực sự được gọi (không vượt oracleMergeSortWorstComparisons(items.size())).
template <class T, class OracleLess>
unsigned long long stableOracleMergeSort(vector<T>& items, OracleLess&& oracleLess) {
    if (items.size() <= 1) return 0;
    vector<T> buffer(items);
    unsigned long long comparisons = 0;

    auto divide = [&](auto&& self, size_t left, size_t right) -> void {
        if (right - left <= 1) return;
        size_t middle = left + (right - left) / 2;
        self(self, left, middle);
        self(self, middle, right);

        size_t i = left, j = middle, out = left;
        while (i < middle && j < right) {
            ++comparisons;
            // Chỉ lấy bản phải khi nó NHỎ HƠN NGẶT; tương đương thì giữ bản trái trước để ổn định.
            if (std::invoke(oracleLess, items[j], items[i])) buffer[out++] = items[j++];
            else buffer[out++] = items[i++];
        }
        while (i < middle) buffer[out++] = items[i++];
        while (j < right) buffer[out++] = items[j++];
        for (size_t k = left; k < right; ++k) items[k] = std::move(buffer[k]);
    };
    divide(divide, 0, items.size());
    return comparisons;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ comparison-sort.hpp -o demo && ./demo
int main() {
    vector<int> hidden = {4, 1, 3, 2};
    vector<int> positions = {0, 1, 2, 3};
    auto queries = stableOracleMergeSort(positions, [&](int i, int j) {
        return hidden[i] < hidden[j];
    });
    printf("order:");
    for (int i : positions) printf(" %d", i);
    printf("; queries=%llu; worst=%llu\n", queries, oracleMergeSortWorstComparisons(4));
    // order: 1 3 2 0; queries=5; worst=5

    vector<pair<int, char>> equalKeys = {{2, 'a'}, {1, 'x'}, {2, 'b'}, {2, 'c'}};
    stableOracleMergeSort(equalKeys, [](const auto& a, const auto& b) { return a.first < b.first; });
    printf("stable=%c%c%c\n", equalKeys[1].second, equalKeys[2].second, equalKeys[3].second);
    // stable=abc
    return 0;
}
#endif
