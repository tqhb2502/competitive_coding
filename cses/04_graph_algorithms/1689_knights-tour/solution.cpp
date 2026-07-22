#include <algorithm>
#include <array>
#include <iostream>
#include <tuple>
#include <vector>

namespace {

constexpr int BOARD_SIZE = 8;
constexpr std::array<int, 8> DR{-2, -2, -1, -1, 1, 1, 2, 2};
constexpr std::array<int, 8> DC{-1, 1, -2, 2, -2, 2, -1, 1};

std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board{};

bool inside(const int row, const int column) {
    return 0 <= row && row < BOARD_SIZE &&
           0 <= column && column < BOARD_SIZE;
}

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

bool find_tour(const int row, const int column, const int step) {
    board[row][column] = step;
    if (step == BOARD_SIZE * BOARD_SIZE) {
        return true;
    }

    // Warnsdorff's rule: visit the square with the fewest onward moves first.
    // For equal degrees, prefer squares farther from the board centre.
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
    std::sort(candidates.begin(), candidates.end());

    for (const auto& [degree, distance, next_row, next_column] : candidates) {
        static_cast<void>(degree);
        static_cast<void>(distance);
        if (find_tour(next_row, next_column, step + 1)) {
            return true;
        }
    }

    board[row][column] = 0;
    return false;
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int x, y;
    std::cin >> x >> y;
    find_tour(y - 1, x - 1, 1);

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
