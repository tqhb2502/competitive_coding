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

    std::vector<int> end_days(n);
    for (int i = 0; i < n; ++i) {
        end_days[i] = projects[i].end;
    }

    // dp[i] is the maximum reward using only the first i projects.
    std::vector<long long> dp(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        // Endpoints are inclusive, so a compatible project must end strictly
        // before the current project's start day.
        const int compatible_count = static_cast<int>(
            std::lower_bound(end_days.begin(), end_days.begin() + i,
                             projects[i].start) -
            end_days.begin());
        const long long take =
            dp[compatible_count] + projects[i].reward;
        dp[i + 1] = std::max(dp[i], take);
    }

    std::cout << dp[n] << '\n';
    return 0;
}
