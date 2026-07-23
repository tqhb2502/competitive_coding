#include <algorithm>
#include <array>
#include <iostream>
#include <tuple>
#include <vector>

namespace {

constexpr int BOARD_SIZE = 8;
// 8 nước đi hình chữ L của quân mã (knight)
constexpr std::array<int, 8> DR{-2, -2, -1, -1, 1, 1, 2, 2};
constexpr std::array<int, 8> DC{-1, 1, -2, 2, -2, 2, -1, 1};

// board[r][c] = thứ tự bước đặt chân tới ô (r, c); 0 nghĩa là chưa thăm
std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board{};

// Kiểm tra ô (row, column) có nằm trong bàn cờ 8x8 không
bool inside(const int row, const int column) {
    return 0 <= row && row < BOARD_SIZE &&
           0 <= column && column < BOARD_SIZE;
}

// Đếm số ô kề chưa thăm của (row, column) - dùng cho heuristic Warnsdorff
int onward_moves(const int row, const int column) {
    int count = 0;
    for (int move = 0; move < 8; ++move) {
        const int next_row = row + DR[move];
        const int next_column = column + DC[move];
        if (inside(next_row, next_column) &&
            board[next_row][next_column] == 0) {
            ++count;
        }
    }
    return count;
}

// DFS backtracking đánh số bước từ 1 tới 64
bool find_tour(const int row, const int column, const int step) {
    board[row][column] = step;
    if (step == BOARD_SIZE * BOARD_SIZE) {
        return true;  // đã đi qua đủ 64 ô, tìm được đường đi Hamilton
    }

    // Heuristic Warnsdorff: thử ô kề có ít nước đi tiếp theo nhất trước.
    // Khi bằng nhau, ưu tiên ô xa tâm bàn cờ hơn (tọa độ nhân đôi để tâm 3.5
    // thành 7, so sánh bằng số nguyên) - phá hòa xác định giúp nhánh đầu luôn
    // thành công nên DFS gần như không phải quay lui.
    std::vector<std::tuple<int, int, int, int>> candidates;
    candidates.reserve(8);
    for (int move = 0; move < 8; ++move) {
        const int next_row = row + DR[move];
        const int next_column = column + DC[move];
        if (!inside(next_row, next_column) ||
            board[next_row][next_column] != 0) {
            continue;
        }

        const int doubled_row = 2 * next_row - (BOARD_SIZE - 1);
        const int doubled_column = 2 * next_column - (BOARD_SIZE - 1);
        const int negative_distance =
            -(doubled_row * doubled_row + doubled_column * doubled_column);
        candidates.emplace_back(
            onward_moves(next_row, next_column),
            negative_distance,
            next_row,
            next_column
        );
    }
    // Sắp xếp: số nước đi tiếp theo tăng dần, rồi tới độ xa tâm giảm dần
    std::sort(candidates.begin(), candidates.end());

    // Thử lần lượt từng ô kề theo thứ tự heuristic
    for (const auto& [degree, distance, next_row, next_column] : candidates) {
        static_cast<void>(degree);
        static_cast<void>(distance);
        if (find_tour(next_row, next_column, step + 1)) {
            return true;
        }
    }

    board[row][column] = 0;  // ngõ cụt: quay lui, đặt lại ô chưa thăm
    return false;
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc ô xuất phát: x là cột, y là hàng (đánh số từ 1)
    int x, y;
    std::cin >> x >> y;
    find_tour(y - 1, x - 1, 1);

    // In bàn cờ 8x8 với thứ tự bước ở mỗi ô
    for (const auto& row : board) {
        for (int column = 0; column < BOARD_SIZE; ++column) {
            if (column != 0) {
                std::cout << ' ';
            }
            std::cout << row[column];
        }
        std::cout << '\n';
    }
    return 0;
}
