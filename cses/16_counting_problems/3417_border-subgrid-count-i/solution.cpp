#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr int WORD_BITS = 64;
constexpr int WORDS_PER_BLOCK = 8;

class ActiveStarts {
public:
    explicit ActiveStarts(int capacity)
        : bits_((capacity + WORD_BITS - 1) / WORD_BITS), word_count_(bits_.size()),
          block_count_((bits_.size() + WORDS_PER_BLOCK - 1) / WORDS_PER_BLOCK) {}

    void clear(int length) {
        const int words = (length + WORD_BITS - 1) / WORD_BITS;
        const int blocks = (words + WORDS_PER_BLOCK - 1) / WORDS_PER_BLOCK;
        std::fill(bits_.begin(), bits_.begin() + words, 0);
        std::fill(word_count_.begin(), word_count_.begin() + words, 0);
        std::fill(block_count_.begin(), block_count_.begin() + blocks, 0);
        total_ = 0;
    }

    void add(int position) {
        const int word = position / WORD_BITS;
        bits_[word] |= std::uint64_t{1} << (position % WORD_BITS);
        ++word_count_[word];
        ++block_count_[word / WORDS_PER_BLOCK];
        ++total_;
    }

    void remove(int position) {
        const int word = position / WORD_BITS;
        bits_[word] &= ~(std::uint64_t{1} << (position % WORD_BITS));
        --word_count_[word];
        --block_count_[word / WORDS_PER_BLOCK];
        --total_;
    }

    [[nodiscard]] int count_from(int left) const {
        const int word = left / WORD_BITS;
        const int block = word / WORDS_PER_BLOCK;
        int before = 0;

        for (int current_block = 0; current_block < block; ++current_block) {
            before += block_count_[current_block];
        }
        for (int current_word = block * WORDS_PER_BLOCK; current_word < word;
             ++current_word) {
            before += word_count_[current_word];
        }

        const int remaining_bits = left % WORD_BITS;
        if (remaining_bits > 0) {
            const std::uint64_t mask = (std::uint64_t{1} << remaining_bits) - 1;
            before += __builtin_popcountll(bits_[word] & mask);
        }

        return total_ - before;
    }

private:
    std::vector<std::uint64_t> bits_;
    std::vector<int> word_count_;
    std::vector<int> block_count_;
    int total_ = 0;
};
}  // namespace

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

    const std::size_t cell_count = static_cast<std::size_t>(n) * n;
    std::vector<std::uint16_t> start_reach(cell_count);
    std::vector<std::uint16_t> finish_reach(cell_count);
    std::vector<int> vertical(n, 0);

    for (int row = n - 1; row >= 0; --row) {
        int horizontal = 0;
        for (int column = n - 1; column >= 0; --column) {
            if (column + 1 < n && grid[row][column] == grid[row][column + 1]) {
                ++horizontal;
            } else {
                horizontal = 1;
            }
            if (row + 1 < n && grid[row][column] == grid[row + 1][column]) {
                ++vertical[column];
            } else {
                vertical[column] = 1;
            }
            start_reach[static_cast<std::size_t>(row) * n + column] =
                static_cast<std::uint16_t>(std::min(horizontal, vertical[column]));
        }
    }

    std::fill(vertical.begin(), vertical.end(), 0);
    for (int row = 0; row < n; ++row) {
        int horizontal = 0;
        for (int column = 0; column < n; ++column) {
            if (column > 0 && grid[row][column] == grid[row][column - 1]) {
                ++horizontal;
            } else {
                horizontal = 1;
            }
            if (row > 0 && grid[row][column] == grid[row - 1][column]) {
                ++vertical[column];
            } else {
                vertical[column] = 1;
            }
            finish_reach[static_cast<std::size_t>(row) * n + column] =
                static_cast<std::uint16_t>(std::min(horizontal, vertical[column]));
        }
    }

    std::vector<std::int64_t> answer(k, 0);
    std::vector<int> expiration_head(n + 1, -1);
    std::vector<int> next_expiring(n, -1);
    ActiveStarts active(n);

    const auto process_diagonal = [&](int first_row, int first_column, int length) {
        std::fill(expiration_head.begin(), expiration_head.begin() + length + 1, -1);
        active.clear(length);

        std::size_t cell = static_cast<std::size_t>(first_row) * n + first_column;
        const std::size_t diagonal_step = static_cast<std::size_t>(n) + 1;

        for (int position = 0; position < length; ++position, cell += diagonal_step) {
            int expiring = expiration_head[position];
            while (expiring != -1) {
                active.remove(expiring);
                expiring = next_expiring[expiring];
            }

            active.add(position);
            const int last = std::min(
                length - 1, position + static_cast<int>(start_reach[cell]) - 1);
            const int expiration = last + 1;
            if (expiration < length) {
                next_expiring[position] = expiration_head[expiration];
                expiration_head[expiration] = position;
            }

            const int left =
                position - static_cast<int>(finish_reach[cell]) + 1;
            const int count = active.count_from(std::max(0, left));
            answer[grid[first_row + position][first_column + position] - 'A'] += count;
        }
    };

    for (int column = 0; column < n; ++column) {
        process_diagonal(0, column, n - column);
    }
    for (int row = 1; row < n; ++row) {
        process_diagonal(row, 0, n - row);
    }

    for (const std::int64_t count : answer) {
        std::cout << count << '\n';
    }
}
