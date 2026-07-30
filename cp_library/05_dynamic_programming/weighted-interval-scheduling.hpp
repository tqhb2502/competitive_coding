#pragma once
#include <bits/stdc++.h>
using namespace std;
// Weighted Interval Scheduling — chọn các khoảng đóng không giao nhau để tối đa tổng trọng số, có truy vết.
// Khi dùng: công việc/dự án có start, finish, profit; nhận việc sau chỉ khi start > finish trước.
// ĐPT: O(n log n) thời gian do sort + lower_bound; O(n) bộ nhớ. Cho phép chọn rỗng với giá trị 0.
// Dùng: auto ans = weightedIntervalSchedule(jobs); ans.value; ans.chosenIds; // theo thứ tự thời gian
// Bẫy: khoảng ĐÓNG [start,finish]; nếu đề cho nửa mở phải đổi điều kiện; tổng profit phải vừa long long.
// CSES: 1140

struct WeightedInterval {
    long long start, finish, weight;
    int id = -1;
};

struct WeightedScheduleResult {
    long long value = 0;
    vector<int> chosenIds;
};

// Ý nghĩa: DP theo finish; với mỗi job, tìm tiền tố kết thúc nghiêm ngặt trước start rồi chọn/bỏ.
// Tham số: jobs = các khoảng đóng và trọng số; id=-1 sẽ được thay bằng vị trí input.
// Trả về: tổng trọng số lớn nhất và id các job đã chọn theo thứ tự finish tăng dần.
inline WeightedScheduleResult weightedIntervalSchedule(vector<WeightedInterval> jobs) {
    for (int i = 0; i < (int)jobs.size(); ++i)
        if (jobs[i].id == -1) jobs[i].id = i;
    sort(jobs.begin(), jobs.end(), [](const auto& left, const auto& right) {
        return tie(left.finish, left.start, left.id)
             < tie(right.finish, right.start, right.id);
    });
    int n = (int)jobs.size();
    vector<long long> finishes(n), dp(n + 1, 0);
    vector<int> previous(n, 0);
    vector<char> take(n, false);
    for (int i = 0; i < n; ++i) finishes[i] = jobs[i].finish;
    for (int i = 0; i < n; ++i) {
        int compatible = (int)(lower_bound(finishes.begin(), finishes.begin() + i,
                                            jobs[i].start) - finishes.begin());
        previous[i] = compatible;
        long long withCurrent = dp[compatible] + jobs[i].weight;
        if (withCurrent > dp[i]) {
            dp[i + 1] = withCurrent;
            take[i] = true;
        } else {
            dp[i + 1] = dp[i];
        }
    }
    WeightedScheduleResult result;
    result.value = dp[n];
    for (int count = n; count > 0;) {
        int i = count - 1;
        if (!take[i] || dp[count] == dp[count - 1]) {
            --count;
        } else {
            result.chosenIds.push_back(jobs[i].id);
            count = previous[i];
        }
    }
    reverse(result.chosenIds.begin(), result.chosenIds.end());
    return result;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ weighted-interval-scheduling.hpp -o demo && ./demo
int main() {
    vector<WeightedInterval> jobs = {{2, 4, 4}, {3, 6, 6}, {6, 8, 2}, {7, 9, 5}};
    auto answer = weightedIntervalSchedule(jobs);
    printf("profit=%lld; jobs:", answer.value);
    for (int id : answer.chosenIds) printf(" %d", id);
    printf("\n");  // profit=11; jobs: 1 3
    return 0;
}
#endif
