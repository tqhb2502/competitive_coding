#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    // height[n] = 0 đóng vai trò cột lính canh để đóng mọi hình chữ nhật còn lại
    std::vector<long long> height(static_cast<std::size_t>(n) + 1U, 0);
    for (int i = 0; i < n; ++i) {
        std::cin >> height[static_cast<std::size_t>(i)];
    }

    // Ngăn xếp đơn điệu lưu các cặp (chiều cao, vị trí trái nhất) tăng dần
    std::vector<std::pair<long long, int>> stack;
    stack.reserve(static_cast<std::size_t>(n) + 1U);
    long long answer = 0;

    for (int i = 0; i <= n; ++i) {
        const long long current = height[static_cast<std::size_t>(i)];
        int left = i;
        // Lấy ra các cột cao hơn current: chúng không thể kéo dài sang phải nữa
        while (!stack.empty() && stack.back().first >= current) {
            const auto [oldHeight, oldLeft] = stack.back();
            stack.pop_back();
            // Cập nhật diện tích hình chữ nhật cao oldHeight, bề rộng i - oldLeft
            answer = std::max(answer, oldHeight * static_cast<long long>(i - oldLeft));
            // Truyền vị trí trái nhất lại cho cột current
            left = oldLeft;
        }
        stack.emplace_back(current, left);
    }

    std::cout << answer << '\n';
    return 0;
}
