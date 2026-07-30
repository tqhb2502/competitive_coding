#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

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

    // run_start[row][column]: chỉ số đầu của đoạn ký tự giống nhau kết thúc tại
    // cột column trên hàng row (dùng để giới hạn cột trái).
    std::vector<std::uint16_t> run_start(static_cast<std::size_t>(n) * n);
    for (int row = 0; row < n; ++row) {
        int start = 0;
        for (int column = 0; column < n; ++column) {
            if (column == 0 || grid[row][column] != grid[row][column - 1]) {
                start = column;
            }
            run_start[static_cast<std::size_t>(row) * n + column] =
                static_cast<std::uint16_t>(start);
        }
    }

    std::vector<std::int64_t> answer(k, 0);
    std::vector<unsigned char> valid_column(n, 1);  // cột có mọi ô [top, bottom] cùng chữ
    std::vector<int> prefix(n + 1, 0);              // prefix sum của các cột hợp lệ

    // Cố định hàng trên top, tăng dần hàng dưới bottom.
    for (int top = 0; top < n; ++top) {
        std::fill(valid_column.begin(), valid_column.end(), 1);

        for (int bottom = top; bottom < n; ++bottom) {
            prefix[0] = 0;
            for (int right = 0; right < n; ++right) {
                // Trạng thái hợp lệ của cột chỉ chuyển từ đúng sang sai khi bottom tăng.
                if (grid[bottom][right] != grid[top][right]) {
                    valid_column[right] = 0;
                }
                prefix[right + 1] = prefix[right] + valid_column[right];

                // Cột phải hợp lệ: đếm số cột trái l hợp lệ và thỏa điều kiện ngang.
                if (valid_column[right] != 0) {
                    const int top_start =
                        run_start[static_cast<std::size_t>(top) * n + right];
                    const int bottom_start =
                        run_start[static_cast<std::size_t>(bottom) * n + right];
                    // Đoạn [l, right] trên cả hàng top và bottom phải toàn cùng một chữ.
                    const int left_limit = std::max(top_start, bottom_start);
                    // Số cách chọn cột trái = số cột hợp lệ trong [left_limit, right].
                    answer[grid[bottom][right] - 'A'] +=
                        prefix[right + 1] - prefix[left_limit];
                }
            }
        }
    }

    for (const std::int64_t count : answer) {
        std::cout << count << '\n';
    }
}
