#pragma once
#include <bits/stdc++.h>
using namespace std;
// Greedy Array — ba mẫu exchange/invariant: làm dãy không giảm, tổng xu thiếu nhỏ nhất, hai máy đọc việc.
// Khi dùng: quyết định trái-phải bắt buộc; các đồng dương đã sort mở rộng đoạn tổng tạo được; cận dưới tải/largest job.
// ĐPT: O(n) trừ smallestMissingSubsetSum O(n log n) do sort; bộ nhớ O(n) khi phải sort bản sao.
// Dùng: minIncrementsNondecreasing(a); smallestMissingSubsetSum(coins); twoProcessorNoOverlapTime(jobs);
// Bẫy: coins phải DƯƠNG; hàm ném overflow_error nếu đáp án không biểu diễn được bằng long long.
// CSES: 1087 1092 1094 1631 1743 2180 2183 3175
// Ý nghĩa: số đơn vị tăng ít nhất để biến dãy thành không giảm khi chỉ được tăng phần tử.
// Tham số: values = dãy đầu vào.
// Trả về: tổng lượng tăng tối thiểu.
long long minIncrementsNondecreasing(const vector<long long>& values) {
    if (values.empty()) return 0;
    long long previous = values[0], answer = 0;
    for (size_t i = 1; i < values.size(); ++i) {
        if (values[i] < previous) {
            __int128 next = (__int128)answer + ((__int128)previous - values[i]);
            if (next > LLONG_MAX)
                throw overflow_error("minIncrementsNondecreasing: đáp án vượt long long");
            answer = (long long)next;
        }
        else previous = values[i];
    }
    return answer;
}

// Ý nghĩa: tìm tổng dương nhỏ nhất không tạo được từ một tập con các đồng xu dương.
// Tham số: coins = giá trị đồng xu (bị sao chép và sort).
// Trả về: tổng thiếu nhỏ nhất.
long long smallestMissingSubsetSum(vector<long long> coins) {
    sort(coins.begin(), coins.end());
    __int128 reach = 0;  // tạo được mọi tổng [0,reach]
    for (long long coin : coins) {
        if (coin <= 0) throw invalid_argument("smallestMissingSubsetSum: coin không dương");
        if (coin > reach + 1) break;
        reach += coin;
        // Từ đây mọi coin còn lại (kiểu long long dương) đều <= reach+1,
        // nên đáp án cuối chắc chắn còn lớn hơn reach.
        if (reach >= LLONG_MAX)
            throw overflow_error("smallestMissingSubsetSum: đáp án vượt long long");
    }
    return (long long)(reach + 1);
}

// Ý nghĩa: thời gian ít nhất để hai người cùng xử lý mọi job, không được xử lý cùng một job đồng thời.
// Tham số: durations = thời lượng dương/không âm của các job.
// Trả về: max(tổng thời lượng, 2*job dài nhất).
long long twoProcessorNoOverlapTime(const vector<long long>& durations) {
    long long sum = 0, longest = 0;
    for (long long value : durations) {
        if (value < 0) throw invalid_argument("twoProcessorNoOverlapTime: thời lượng âm");
        if (sum > LLONG_MAX - value) throw overflow_error("twoProcessorNoOverlapTime: tràn tổng");
        sum += value;
        longest = max(longest, value);
    }
    if (longest > LLONG_MAX / 2) throw overflow_error("twoProcessorNoOverlapTime: tràn 2*max");
    return max(sum, 2 * longest);
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ greedy-array.hpp -o demo && ./demo
int main() {
    printf("increments=%lld missing=%lld reading=%lld\n",
           minIncrementsNondecreasing({3, 2, 5, 1, 7}),
           smallestMissingSubsetSum({1, 1, 3, 4}),
           twoProcessorNoOverlapTime({2, 8, 3}));  // 5, 10, 16
    return 0;
}
#endif
