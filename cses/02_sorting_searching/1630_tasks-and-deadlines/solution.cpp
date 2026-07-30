#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Mỗi công việc lưu cặp (thời gian a, deadline d); dùng 64-bit vì đáp án lớn.
    vector<pair<long long, long long>> tasks(n);
    for (auto& [duration, deadline] : tasks) {
        cin >> duration >> deadline;
    }

    // Greedy Shortest Processing Time: sắp xếp theo thời gian a tăng dần
    // để tối thiểu tổng các thời điểm kết thúc sum(f), nhờ đó tối đa tổng phần thưởng.
    sort(tasks.begin(), tasks.end());

    // Duyệt theo thứ tự đã sắp: cộng dồn thời gian để có thời điểm kết thúc,
    // rồi cộng phần thưởng (d - f) của từng công việc.
    long long current_time = 0;
    long long reward = 0;
    for (const auto& [duration, deadline] : tasks) {
        current_time += duration;
        reward += deadline - current_time;
    }

    cout << reward << '\n';
    return 0;
}
