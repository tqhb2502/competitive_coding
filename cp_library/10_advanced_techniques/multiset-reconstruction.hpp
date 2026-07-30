#pragma once
#include <bits/stdc++.h>
using namespace std;
// Multiset Reconstruction — dựng một dãy từ multiset tổng của mọi cặp chỉ số khác nhau.
// Khi dùng: biết n và n(n-1)/2 pair sums (có lặp); cần tìm bất kỳ dãy phù hợp hoặc kết luận vô nghiệm.
// ĐPT: O(n^3 log n) theo cận thô, gồm sort; bộ nhớ O(n^2).
// Dùng: auto a = reconstructFromPairSums(4, sums, 1, 1000000000LL);
// Bẫy: yêu cầu n>=3; output được sort không giảm; minValue/maxValue là cận ĐÓNG;
//       mọi phần tử và pair sum phải vừa long long, phép trung gian dùng __int128.
// CSES: 2414

// Ý nghĩa: dựng một dãy có multiset tổng cặp đúng bằng đầu vào và thỏa cận giá trị.
// Tham số: n = số phần tử (>=3); pairSums = đúng n(n-1)/2 tổng a[i]+a[j], i<j;
//          minValue, maxValue = cận đóng cho từng a[i].
// Trả về: một dãy n phần tử sort không giảm nếu tồn tại; nullopt nếu kích thước/multiset không phù hợp.
optional<vector<long long>> reconstructFromPairSums(
        int n, const vector<long long>& pairSums,
        long long minValue = LLONG_MIN, long long maxValue = LLONG_MAX) {
    if (n < 3) throw invalid_argument("reconstructFromPairSums: cần n >= 3");
    if (minValue > maxValue)
        throw invalid_argument("reconstructFromPairSums: cận giá trị rỗng");
    __int128 expected = (__int128)n * (n - 1) / 2;
    if (expected != (__int128)pairSums.size()) return nullopt;

    vector<long long> sortedSums = pairSums;
    sort(sortedSums.begin(), sortedSums.end());
    int candidateCount = n;  // Với n>=3, n(n-1)/2 >= n và kích thước đã được kiểm tra.
    set<long long> triedFirst;

    for (int candidate = 2; candidate < candidateCount; ++candidate) {
        __int128 twiceFirst = (__int128)sortedSums[0] + sortedSums[1]
                            - sortedSums[candidate];
        if (twiceFirst % 2 != 0) continue;
        __int128 first128 = twiceFirst / 2;
        if (first128 < (__int128)minValue || first128 > (__int128)maxValue) continue;
        long long first = (long long)first128;
        if (!triedFirst.insert(first).second) continue;

        multiset<long long> remaining(sortedSums.begin(), sortedSums.end());
        vector<long long> answer{first};
        answer.reserve(n);
        bool valid = true;
        while ((int)answer.size() < n) {
            if (remaining.empty()) {
                valid = false;
                break;
            }
            __int128 next128 = (__int128)*remaining.begin() - first;
            if (next128 < (__int128)minValue || next128 > (__int128)maxValue ||
                next128 < (__int128)answer.back()) {
                valid = false;
                break;
            }
            long long nextValue = (long long)next128;
            for (long long value : answer) {
                __int128 sum128 = (__int128)value + nextValue;
                if (sum128 < (__int128)LLONG_MIN || sum128 > (__int128)LLONG_MAX) {
                    valid = false;
                    break;
                }
                auto found = remaining.find((long long)sum128);
                if (found == remaining.end()) {
                    valid = false;
                    break;
                }
                remaining.erase(found);
            }
            if (!valid) break;
            answer.push_back(nextValue);
        }
        if (valid && remaining.empty()) return answer;
    }
    return nullopt;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ multiset-reconstruction.hpp -o demo && ./demo
int main() {
    vector<long long> sums = {3, 4, 5, 5, 6, 7};  // dãy {1,2,3,4}
    auto answer = reconstructFromPairSums(4, sums, 1, 1000000000LL);
    printf("day:");
    if (answer) for (long long value : *answer) printf(" %lld", value);
    printf("\n");  // 1 2 3 4
    return 0;
}
#endif
