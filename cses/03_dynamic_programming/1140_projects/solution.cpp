#include <algorithm>
#include <iostream>
#include <vector>

struct Project {
    int end;
    int start;
    long long reward;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<Project> projects(n);
    for (Project& project : projects) {
        std::cin >> project.start >> project.end >> project.reward;
    }

    // Sắp xếp các dự án theo ngày kết thúc tăng dần (weighted interval scheduling).
    std::sort(projects.begin(), projects.end(),
              [](const Project& lhs, const Project& rhs) {
                  if (lhs.end != rhs.end) {
                      return lhs.end < rhs.end;
                  }
                  if (lhs.start != rhs.start) {
                      return lhs.start < rhs.start;
                  }
                  return lhs.reward < rhs.reward;
              });

    // Mảng các ngày kết thúc đã sắp xếp, dùng cho tìm kiếm nhị phân.
    std::vector<int> end_days(n);
    for (int i = 0; i < n; ++i) {
        end_days[i] = projects[i].end;
    }

    // dp[i] = tổng tiền thưởng lớn nhất khi chỉ xét i dự án đầu tiên.
    std::vector<long long> dp(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        // Ngày là bao gồm cả hai đầu mút, nên dự án tương thích phải kết thúc
        // trước ngày bắt đầu của dự án hiện tại; đếm số dự án đó bằng lower_bound.
        const int compatible_count = static_cast<int>(
            std::lower_bound(end_days.begin(), end_days.begin() + i,
                             projects[i].start) -
            end_days.begin());
        // Chọn phương án tốt hơn: nhận dự án i hay bỏ qua nó.
        const long long take =
            dp[compatible_count] + projects[i].reward;
        dp[i + 1] = std::max(dp[i], take);
    }

    std::cout << dp[n] << '\n';
    return 0;
}
