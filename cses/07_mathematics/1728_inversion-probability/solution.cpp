#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> ranges(static_cast<std::size_t>(n));
    for (int& range : ranges) {
        std::cin >> range;
    }

    double answer = 0.0;
    for (std::size_t i = 0; i < ranges.size(); ++i) {
        for (std::size_t j = i + 1; j < ranges.size(); ++j) {
            const long long upper = std::min(ranges[j], ranges[i] - 1);
            if (upper <= 0) {
                continue;
            }

            // Sum_{v=1..upper} (ranges[i] - v).
            const long long favorable =
                upper * ranges[i] - upper * (upper + 1) / 2;
            answer += static_cast<double>(favorable) /
                      static_cast<double>(ranges[i] * ranges[j]);
        }
    }

    std::cout << std::fixed << std::setprecision(6) << answer << '\n';
    return 0;
}
