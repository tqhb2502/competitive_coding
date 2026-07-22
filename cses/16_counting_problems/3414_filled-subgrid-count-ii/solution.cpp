#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct HeightGroup {
    int height;
    int width_count;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<int> height(n, 0);
    std::vector<char> previous(n, '#');
    std::vector<std::int64_t> answer(k, 0);
    std::vector<HeightGroup> stack;
    stack.reserve(n);

    std::string row;
    for (int r = 0; r < n; ++r) {
        std::cin >> row;
        std::int64_t current_sum = 0;
        stack.clear();

        for (int column = 0; column < n; ++column) {
            if (row[column] == previous[column]) {
                ++height[column];
            } else {
                height[column] = 1;
            }
            previous[column] = row[column];

            if (column > 0 && row[column] != row[column - 1]) {
                stack.clear();
                current_sum = 0;
            }

            int width_count = 1;
            while (!stack.empty() && stack.back().height >= height[column]) {
                current_sum -= static_cast<std::int64_t>(stack.back().height) *
                               stack.back().width_count;
                width_count += stack.back().width_count;
                stack.pop_back();
            }

            stack.push_back({height[column], width_count});
            current_sum += static_cast<std::int64_t>(height[column]) * width_count;
            answer[row[column] - 'A'] += current_sum;
        }
    }

    for (const std::int64_t count : answer) {
        std::cout << count << '\n';
    }
}
