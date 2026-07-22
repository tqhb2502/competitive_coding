#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<long long> height(static_cast<std::size_t>(n) + 1U, 0);
    for (int i = 0; i < n; ++i) {
        std::cin >> height[static_cast<std::size_t>(i)];
    }

    std::vector<std::pair<long long, int>> stack;
    stack.reserve(static_cast<std::size_t>(n) + 1U);
    long long answer = 0;

    for (int i = 0; i <= n; ++i) {
        const long long current = height[static_cast<std::size_t>(i)];
        int left = i;
        while (!stack.empty() && stack.back().first >= current) {
            const auto [oldHeight, oldLeft] = stack.back();
            stack.pop_back();
            answer = std::max(answer, oldHeight * static_cast<long long>(i - oldLeft));
            left = oldLeft;
        }
        stack.emplace_back(current, left);
    }

    std::cout << answer << '\n';
    return 0;
}
