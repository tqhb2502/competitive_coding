#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Một nhóm trên ngăn xếp đơn điệu: cùng giá trị height nhỏ nhất
// và số cột mà nhóm đó đại diện.
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

    std::vector<int> height(n, 0);       // chiều cao đoạn cùng chữ đi lên từ mỗi cột
    std::vector<char> previous(n, '#');  // ký tự của hàng trước ở mỗi cột
    std::vector<std::int64_t> answer(k, 0);
    std::vector<HeightGroup> stack;
    stack.reserve(n);

    std::string row;
    for (int r = 0; r < n; ++r) {
        std::cin >> row;
        std::int64_t current_sum = 0;
        stack.clear();

        for (int column = 0; column < n; ++column) {
            // Cập nhật chiều cao: nối tiếp nếu cùng chữ với hàng trên, ngược lại reset về 1.
            if (row[column] == previous[column]) {
                ++height[column];
            } else {
                height[column] = 1;
            }
            previous[column] = row[column];

            // Ký tự đổi trên hàng hiện tại: bắt đầu một đoạn mới, xóa ngăn xếp.
            if (column > 0 && row[column] != row[column - 1]) {
                stack.clear();
                current_sum = 0;
            }

            // Gộp các nhóm có height >= height hiện tại, trừ đóng góp cũ của chúng.
            int width_count = 1;
            while (!stack.empty() && stack.back().height >= height[column]) {
                current_sum -= static_cast<std::int64_t>(stack.back().height) *
                               stack.back().width_count;
                width_count += stack.back().width_count;
                stack.pop_back();
            }

            // Đẩy nhóm mới và cộng đóng góp height * số cột đại diện.
            stack.push_back({height[column], width_count});
            current_sum += static_cast<std::int64_t>(height[column]) * width_count;

            // current_sum là số hình chữ nhật đồng màu có góc dưới phải tại ô này.
            answer[row[column] - 'A'] += current_sum;
        }
    }

    for (const std::int64_t count : answer) {
        std::cout << count << '\n';
    }
}
