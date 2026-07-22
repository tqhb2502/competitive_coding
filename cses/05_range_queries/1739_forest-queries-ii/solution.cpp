#include <iostream>
#include <string>
#include <vector>

class Fenwick2D {
public:
    Fenwick2D(int n, const std::vector<std::string>& grid)
        : n_(n), width_(n + 1), data_(static_cast<std::size_t>(width_ * width_), 0) {
        for (int row = 1; row <= n_; ++row) {
            for (int column = 1; column <= n_; ++column) {
                data_[index(row, column)] =
                    grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(column - 1)] == '*';
            }
        }

        // Build in O(n^2): first propagate each row, then each column.
        for (int row = 1; row <= n_; ++row) {
            for (int column = 1; column <= n_; ++column) {
                const int parent = column + (column & -column);
                if (parent <= n_) {
                    data_[index(row, parent)] += data_[index(row, column)];
                }
            }
        }
        for (int column = 1; column <= n_; ++column) {
            for (int row = 1; row <= n_; ++row) {
                const int parent = row + (row & -row);
                if (parent <= n_) {
                    data_[index(parent, column)] += data_[index(row, column)];
                }
            }
        }
    }

    void add(int row, int column, int value) {
        for (int i = row; i <= n_; i += i & -i) {
            for (int j = column; j <= n_; j += j & -j) {
                data_[index(i, j)] += value;
            }
        }
    }

    [[nodiscard]] int rectangle_sum(int row1, int column1, int row2, int column2) const {
        return prefix_sum(row2, column2) - prefix_sum(row1 - 1, column2) -
               prefix_sum(row2, column1 - 1) + prefix_sum(row1 - 1, column1 - 1);
    }

private:
    int n_;
    int width_;
    std::vector<int> data_;

    [[nodiscard]] std::size_t index(int row, int column) const {
        return static_cast<std::size_t>(row * width_ + column);
    }

    [[nodiscard]] int prefix_sum(int row, int column) const {
        int result = 0;
        for (int i = row; i > 0; i -= i & -i) {
            for (int j = column; j > 0; j -= j & -j) {
                result += data_[index(i, j)];
            }
        }
        return result;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;
    std::vector<std::string> grid(static_cast<std::size_t>(n + 1));
    for (int row = 1; row <= n; ++row) {
        std::cin >> grid[static_cast<std::size_t>(row)];
    }

    Fenwick2D tree(n, grid);
    while (q-- > 0) {
        int type = 0;
        std::cin >> type;
        if (type == 1) {
            int row = 0;
            int column = 0;
            std::cin >> row >> column;
            char& cell = grid[static_cast<std::size_t>(row)][static_cast<std::size_t>(column - 1)];
            if (cell == '*') {
                cell = '.';
                tree.add(row, column, -1);
            } else {
                cell = '*';
                tree.add(row, column, 1);
            }
        } else {
            int row1 = 0;
            int column1 = 0;
            int row2 = 0;
            int column2 = 0;
            std::cin >> row1 >> column1 >> row2 >> column2;
            std::cout << tree.rectangle_sum(row1, column1, row2, column2) << '\n';
        }
    }
}
