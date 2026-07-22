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
    std::vector<unsigned char> valid_column(n, 1);
    std::vector<int> prefix(n + 1, 0);

    for (int top = 0; top < n; ++top) {
        std::fill(valid_column.begin(), valid_column.end(), 1);

        for (int bottom = top; bottom < n; ++bottom) {
            prefix[0] = 0;
            for (int right = 0; right < n; ++right) {
                if (grid[bottom][right] != grid[top][right]) {
                    valid_column[right] = 0;
                }
                prefix[right + 1] = prefix[right] + valid_column[right];

                if (valid_column[right] != 0) {
                    const int top_start =
                        run_start[static_cast<std::size_t>(top) * n + right];
                    const int bottom_start =
                        run_start[static_cast<std::size_t>(bottom) * n + right];
                    const int left_limit = std::max(top_start, bottom_start);
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
