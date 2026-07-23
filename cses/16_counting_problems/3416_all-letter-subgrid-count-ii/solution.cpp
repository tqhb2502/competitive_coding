#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Hàng đợi hỗ trợ lấy OR của toàn bộ phần tử (phép OR không có nghịch đảo nên
// cài bằng hai ngăn xếp, mỗi ngăn xếp lưu OR tích lũy).
class OrQueue {
public:
    explicit OrQueue(int capacity)
        : front_value_(capacity), front_or_(capacity), back_value_(capacity),
          back_or_(capacity) {}

    void clear() {
        front_size_ = 0;
        back_size_ = 0;
    }

    // Thêm giá trị vào ngăn xếp sau và cập nhật OR tích lũy.
    void push(std::uint32_t value) {
        back_value_[back_size_] = value;
        back_or_[back_size_] = value;
        if (back_size_ > 0) {
            back_or_[back_size_] |= back_or_[back_size_ - 1];
        }
        ++back_size_;
    }

    // Xóa phần tử đầu; nếu ngăn xếp trước rỗng thì chuyển toàn bộ từ sau sang.
    void pop() {
        if (front_size_ == 0) {
            transfer_to_front();
        }
        --front_size_;
    }

    // OR của cả hàng đợi = OR hai giá trị tích lũy trên đỉnh hai ngăn xếp.
    [[nodiscard]] std::uint32_t aggregate() const {
        const std::uint32_t front = front_size_ == 0 ? 0 : front_or_[front_size_ - 1];
        const std::uint32_t back = back_size_ == 0 ? 0 : back_or_[back_size_ - 1];
        return front | back;
    }

private:
    // Đổ toàn bộ ngăn xếp sau sang ngăn xếp trước (đảo thứ tự), dựng lại OR tích lũy.
    void transfer_to_front() {
        while (back_size_ > 0) {
            --back_size_;
            const std::uint32_t value = back_value_[back_size_];
            front_value_[front_size_] = value;
            front_or_[front_size_] = value;
            if (front_size_ > 0) {
                front_or_[front_size_] |= front_or_[front_size_ - 1];
            }
            ++front_size_;
        }
    }

    std::vector<std::uint32_t> front_value_;
    std::vector<std::uint32_t> front_or_;
    std::vector<std::uint32_t> back_value_;
    std::vector<std::uint32_t> back_or_;
    int front_size_ = 0;
    int back_size_ = 0;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<std::string> grid(n);
    for (std::string& row : grid) {
        std::cin >> row;
    }

    // full_mask có đúng k bit 1: một cửa sổ đủ chữ khi OR bằng giá trị này.
    const std::uint32_t full_mask = (std::uint32_t{1} << k) - 1;
    std::vector<std::uint32_t> column_mask(n, 0);
    OrQueue window(n);
    std::int64_t answer = 0;

    // Cố định hàng trên top, mở rộng dần hàng dưới bottom.
    for (int top = 0; top < n; ++top) {
        std::fill(column_mask.begin(), column_mask.end(), 0);

        for (int bottom = top; bottom < n; ++bottom) {
            // Bổ sung chữ cái của hàng bottom vào bitmask của từng cột.
            for (int column = 0; column < n; ++column) {
                column_mask[column] |=
                    std::uint32_t{1} << (grid[bottom][column] - 'A');
            }

            // Hai con trỏ: với mỗi cột trái left, tìm right nhỏ nhất để cửa sổ đủ chữ.
            window.clear();
            int right = 0;
            for (int left = 0; left < n; ++left) {
                while (right < n && window.aggregate() != full_mask) {
                    window.push(column_mask[right]);
                    ++right;
                }

                // Nếu đủ chữ thì mọi cột phải từ right-1 tới n-1 đều hợp lệ.
                if (window.aggregate() == full_mask) {
                    answer += n - right + 1;
                }
                window.pop();
            }
        }
    }

    std::cout << answer << '\n';
}
