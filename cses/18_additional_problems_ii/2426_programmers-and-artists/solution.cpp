#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // programmers = a lập trình viên, artists = b họa sĩ cần tuyển
    int programmers;
    int artists;
    int applicantCount;
    cin >> programmers >> artists >> applicantCount;

    // Mỗi ứng viên có kỹ năng (x = lập trình, y = họa)
    vector<pair<long long, long long>> applicants(applicantCount);
    for (auto& [programmingSkill, artisticSkill] : applicants) {
        cin >> programmingSkill >> artisticSkill;
    }
    // Sắp giảm dần theo x - y để tồn tại một điểm chia giữa hai vai trò
    sort(applicants.begin(), applicants.end(),
         [](const auto& first, const auto& second) {
             return first.first - first.second >
                    second.first - second.second;
         });

    const long long impossible = numeric_limits<long long>::min() / 4;

    // prefix[i]: tổng x lớn nhất khi chọn đúng a lập trình viên trong [0, i)
    // Dùng min-heap kích thước a để luôn giữ lại a giá trị x lớn nhất
    vector<long long> prefix(applicantCount + 1, impossible);
    priority_queue<long long, vector<long long>, greater<long long>> heap;
    long long sum = 0;
    for (int index = 0; index <= applicantCount; ++index) {
        // Đủ đúng a người thì ghi lại tổng hiện tại
        if (static_cast<int>(heap.size()) == programmers) {
            prefix[index] = sum;
        }
        if (index == applicantCount) {
            break;
        }
        heap.push(applicants[index].first);
        sum += applicants[index].first;
        // Vượt quá a người thì bỏ giá trị x nhỏ nhất ra khỏi heap
        if (static_cast<int>(heap.size()) > programmers) {
            sum -= heap.top();
            heap.pop();
        }
    }

    // suffix[i]: tổng y lớn nhất khi chọn đúng b họa sĩ trong [i, n)
    vector<long long> suffix(applicantCount + 1, impossible);
    heap = priority_queue<long long, vector<long long>, greater<long long>>();
    sum = 0;
    for (int index = applicantCount; index >= 0; --index) {
        // Đủ đúng b người thì ghi lại tổng hiện tại
        if (static_cast<int>(heap.size()) == artists) {
            suffix[index] = sum;
        }
        if (index == 0) {
            break;
        }
        heap.push(applicants[index - 1].second);
        sum += applicants[index - 1].second;
        // Vượt quá b người thì bỏ giá trị y nhỏ nhất ra khỏi heap
        if (static_cast<int>(heap.size()) > artists) {
            sum -= heap.top();
            heap.pop();
        }
    }

    // Xét mọi điểm chia hợp lệ, lấy tổng prefix + suffix lớn nhất
    long long answer = 0;
    for (int split = 0; split <= applicantCount; ++split) {
        if (prefix[split] != impossible && suffix[split] != impossible) {
            answer = max(answer, prefix[split] + suffix[split]);
        }
    }
    cout << answer << '\n';
}
