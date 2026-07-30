#pragma once
#include <bits/stdc++.h>
using namespace std;
// Heap Selection — tổng top-k trên mọi prefix/suffix, chọn hai vai trò qua điểm chia, và mô phỏng k vòng bubble bằng heap lookahead.
// Khi dùng: thử mọi split nhưng mỗi phía phải giữ k điểm lớn nhất; lấy phần tử tốt nhất trong cửa sổ ứng viên động; k rất lớn không thể mô phỏng vòng.
// ĐPT: top-k O(n*(1+log(min(n,k)+1))), O(n+min(n,k)); two-role O(n log n), O(n); bubble O(n*(1+log(min(n,passes+1)))), O(n).
// Dùng: auto pre=largestKPrefixSums(a,k); auto suf=largestKSuffixSums(a,k); maximumSeparatedTwoRoleScore(scores,A,B);
//        auto after = stableBubblePassesByHeap(values, passes);  // bubble tăng dần, ổn định
// Bẫy: output top-k dài n+1 dùng đoạn NỬA MỞ [0,i) / [i,n); nullopt nghĩa là chưa đủ k phần tử, k=0 luôn có tổng 0.
//       Tổng phải vừa long long (hàm kiểm tra và ném overflow_error); bubble chỉ swap khi phần tử trái lớn hơn nên giữ thứ tự các giá trị tương đương.
// CSES: 2426 3152

// Ý nghĩa: tính tổng k giá trị lớn nhất trong mọi prefix nửa mở [0,i) bằng min-heap kích thước k.
// Tham số: values = mảng điểm long long; k = số phần tử cần chọn, phải không âm.
// Trả về: vector n+1 optional; result[i] có giá trị iff i>=k, k=0 cho 0 tại mọi i.
inline vector<optional<long long>> largestKPrefixSums(
    const vector<long long>& values, int k) {
    if (k < 0) throw invalid_argument("largestKPrefixSums: k âm");
    int n = (int)values.size();
    vector<optional<long long>> result(n + 1);
    if (k == 0) {
        fill(result.begin(), result.end(), optional<long long>(0));
        return result;
    }
    priority_queue<long long, vector<long long>, greater<long long>> selected;
    __int128 sum = 0;
    for (int i = 0; i < n; ++i) {
        selected.push(values[i]);
        sum += values[i];
        if ((int)selected.size() > k) {
            sum -= selected.top();
            selected.pop();
        }
        if ((int)selected.size() == k) {
            if (sum < numeric_limits<long long>::min() ||
                sum > numeric_limits<long long>::max())
                throw overflow_error("largestKPrefixSums: tổng vượt long long");
            result[i + 1] = (long long)sum;
        }
    }
    return result;
}

// Ý nghĩa: tính tổng k giá trị lớn nhất trong mọi suffix nửa mở [i,n) bằng min-heap kích thước k.
// Tham số: values = mảng điểm long long; k = số phần tử cần chọn, phải không âm.
// Trả về: vector n+1 optional; result[i] có giá trị iff n-i>=k, k=0 cho 0 tại mọi i.
inline vector<optional<long long>> largestKSuffixSums(
    const vector<long long>& values, int k) {
    if (k < 0) throw invalid_argument("largestKSuffixSums: k âm");
    vector<long long> reversed(values.rbegin(), values.rend());
    auto reversedPrefixes = largestKPrefixSums(reversed, k);
    vector<optional<long long>> result(values.size() + 1);
    for (size_t i = 0; i <= values.size(); ++i) {
        result[i] = reversedPrefixes[values.size() - i];
    }
    return result;
}

// Ý nghĩa: chọn leftCount người lấy điểm first và rightCount người khác lấy điểm second, tối đa tổng bằng sort theo first-second rồi thử split.
// Tham số: scores = {điểm vai trái, điểm vai phải}; leftCount/rightCount = số người cần cho từng vai, phải không âm.
// Trả về: tổng tối đa, hoặc nullopt nếu leftCount+rightCount > số người; ném overflow_error nếu đáp án không vừa long long.
inline optional<long long> maximumSeparatedTwoRoleScore(
    vector<pair<long long, long long>> scores, int leftCount, int rightCount) {
    if (leftCount < 0 || rightCount < 0)
        throw invalid_argument("maximumSeparatedTwoRoleScore: số lượng âm");
    int n = (int)scores.size();
    if ((long long)leftCount + rightCount > n) return nullopt;
    stable_sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return (__int128)a.first - a.second > (__int128)b.first - b.second;
    });

    vector<long long> leftScore(n), rightScore(n);
    for (int i = 0; i < n; ++i) {
        leftScore[i] = scores[i].first;
        rightScore[i] = scores[i].second;
    }
    auto prefix = largestKPrefixSums(leftScore, leftCount);
    auto suffix = largestKSuffixSums(rightScore, rightCount);
    optional<long long> best;
    for (int split = leftCount; split + rightCount <= n; ++split) {
        if (!prefix[split] || !suffix[split]) continue;
        __int128 candidate = (__int128)*prefix[split] + *suffix[split];
        if (candidate < numeric_limits<long long>::min() ||
            candidate > numeric_limits<long long>::max())
            throw overflow_error("maximumSeparatedTwoRoleScore: tổng vượt long long");
        long long value = (long long)candidate;
        if (!best || value > *best) best = value;
    }
    return best;
}

// Ý nghĩa: trả đúng mảng sau passes vòng bubble sort trái->phải bằng cách mỗi lần lấy phần tử nhỏ nhất trong passes+1 ứng viên chưa lấy đầu tiên.
// Tham số: values = mảng gốc; passes = số vòng không âm; less = strict-weak-order xác định thứ tự tăng.
// Trả về: mảng sau đúng passes vòng; phần tử tương đương giữ thứ tự đầu vào nhờ tie theo index.
template <class T, class Less = std::less<T>>
vector<T> stableBubblePassesByHeap(
    const vector<T>& values, long long passes, Less less = Less()) {
    if (passes < 0) throw invalid_argument("stableBubblePassesByHeap: passes âm");
    size_t n = values.size();
    if (n == 0) return {};
    size_t window = passes >= (long long)n - 1 ? n : (size_t)passes + 1;

    auto lowerPriority = [&](size_t i, size_t j) {
        if (std::invoke(less, values[j], values[i])) return true;   // values[i] lớn hơn.
        if (std::invoke(less, values[i], values[j])) return false;
        return i > j;  // tương đương: index nhỏ hơn được lấy trước để ổn định.
    };
    priority_queue<size_t, vector<size_t>, decltype(lowerPriority)> candidates(lowerPriority);
    for (size_t i = 0; i < window; ++i) candidates.push(i);

    vector<T> answer;
    answer.reserve(n);
    size_t next = window;
    while (!candidates.empty()) {
        size_t chosen = candidates.top();
        candidates.pop();
        answer.push_back(values[chosen]);
        if (next < n) candidates.push(next++);
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ heap-selection.hpp -o demo && ./demo
int main() {
    auto prefix = largestKPrefixSums({5, 1, 4, 2}, 2);
    auto suffix = largestKSuffixSums({5, 1, 4, 2}, 2);
    printf("pre4=%lld suf1=%lld; roles=%lld\n",
           *prefix[4], *suffix[1],
           *maximumSeparatedTwoRoleScore({{10, 1}, {8, 7}, {2, 9}}, 1, 1));
    // pre4=9 suf1=6; roles=19

    auto after = stableBubblePassesByHeap(vector<int>{4, 3, 2, 1}, 2);
    printf("bubble:");
    for (int x : after) printf(" %d", x);
    printf("\n");  // bubble: 2 1 3 4
    return 0;
}
#endif
