#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>

namespace {
constexpr int REACH_BITS = 12;
constexpr std::uint32_t REACH_MASK = (1U << REACH_BITS) - 1;
constexpr int FINISH_SHIFT = REACH_BITS;
constexpr int COLOR_SHIFT = 2 * REACH_BITS;

constexpr int WORD_BITS = 64;
constexpr int WORDS_PER_BLOCK = 8;

class FastInput {
public:
    int read_int() {
        char character = read_character();
        while (character < '0' || character > '9') {
            character = read_character();
        }

        int value = 0;
        while (character >= '0' && character <= '9') {
            value = value * 10 + character - '0';
            character = read_character();
        }
        return value;
    }

    void read_row(std::string& row, int length) {
        row.resize(length);
        char character = read_character();
        while (character <= ' ') {
            character = read_character();
        }

        row[0] = character;
        for (int column = 1; column < length; ++column) {
            row[column] = read_character();
        }
    }

private:
    static constexpr std::size_t BUFFER_SIZE = 1U << 16;

    char read_character() {
        if (position_ == size_) {
            size_ = std::fread(buffer_, 1, BUFFER_SIZE, stdin);
            position_ = 0;
        }
        return buffer_[position_++];
    }

    char buffer_[BUFFER_SIZE];
    std::size_t position_ = 0;
    std::size_t size_ = 0;
};

class ExpiredStarts {
public:
    explicit ExpiredStarts(int capacity)
        : bits_((capacity + WORD_BITS - 1) / WORD_BITS),
          word_count_(bits_.size()),
          block_count_((bits_.size() + WORDS_PER_BLOCK - 1) /
                       WORDS_PER_BLOCK) {}

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

    // All expired positions are smaller than right. Count the expired positions
    // in [left, right), using whichever side of the interval is cheaper to scan.
    [[nodiscard]] int count_between(int left, int right) const {
        if (total_ == 0 || left >= right) {
            return 0;
        }

        const int first_word = left / WORD_BITS;
        const int last_word = (right - 1) / WORD_BITS;
        const int direct_cost = last_word - first_word + 1;
        const int first_block = first_word / WORDS_PER_BLOCK;
        const int prefix_cost = first_block + first_word % WORDS_PER_BLOCK +
                                (left % WORD_BITS != 0);

        if (direct_cost <= prefix_cost) {
            const int left_bit = left % WORD_BITS;
            const int right_bit = right % WORD_BITS;
            const std::uint64_t left_mask = ~std::uint64_t{0} << left_bit;
            const std::uint64_t right_mask =
                right_bit == 0
                    ? ~std::uint64_t{0}
                    : (std::uint64_t{1} << right_bit) - 1;

            if (first_word == last_word) {
                return __builtin_popcountll(bits_[first_word] & left_mask &
                                            right_mask);
            }

            int count = __builtin_popcountll(bits_[first_word] & left_mask);
            for (int word = first_word + 1; word < last_word; ++word) {
                count += word_count_[word];
            }
            count += __builtin_popcountll(bits_[last_word] & right_mask);
            return count;
        }

        int before = 0;
        for (int block = 0; block < first_block; ++block) {
            before += block_count_[block];
        }
        for (int word = first_block * WORDS_PER_BLOCK; word < first_word;
             ++word) {
            before += word_count_[word];
        }
        if (left % WORD_BITS != 0) {
            const std::uint64_t mask =
                (std::uint64_t{1} << (left % WORD_BITS)) - 1;
            before += __builtin_popcountll(bits_[first_word] & mask);
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
    FastInput input;
    const int n = input.read_int();
    const int k = input.read_int();

    std::vector<std::string> grid(n);
    for (std::string& row : grid) {
        input.read_row(row, n);
    }

    const std::size_t cell_count = static_cast<std::size_t>(n) * n;
    std::vector<std::uint32_t> cell_info(cell_count);
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
            cell_info[static_cast<std::size_t>(row) * n + column] =
                std::min(horizontal, vertical[column]);
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

            const std::size_t cell = static_cast<std::size_t>(row) * n + column;
            const std::uint32_t finish = std::min(horizontal, vertical[column]);
            const std::uint32_t color = grid[row][column] - 'A';
            cell_info[cell] |= finish << FINISH_SHIFT;
            cell_info[cell] |= color << COLOR_SHIFT;
        }
    }

    std::vector<std::int64_t> answer(k, 0);
    std::vector<int> expiration_head(n + 1, -1);
    std::vector<int> next_expiring(n, -1);
    std::vector<std::uint32_t> diagonal(n);
    ExpiredStarts expired(n);

    const auto process_diagonal = [&](int first_row, int first_column, int length) {
        std::fill(expiration_head.begin(), expiration_head.begin() + length + 1, -1);
        expired.clear(length);

        std::size_t cell = static_cast<std::size_t>(first_row) * n + first_column;
        const std::size_t diagonal_step = static_cast<std::size_t>(n) + 1;
        for (int position = 0; position < length;
             ++position, cell += diagonal_step) {
            diagonal[position] = cell_info[cell];
        }

        for (int position = 0; position < length; ++position) {
            int expiring = expiration_head[position];
            while (expiring != -1) {
                expired.add(expiring);
                expiring = next_expiring[expiring];
            }

            const std::uint32_t info = diagonal[position];
            const int start = info & REACH_MASK;
            const int finish = (info >> FINISH_SHIFT) & REACH_MASK;
            const int left = std::max(0, position - finish + 1);
            const int candidates = position - left + 1;
            const int invalid = expired.count_between(left, position);
            const int color = info >> COLOR_SHIFT;
            answer[color] += candidates - invalid;

            const int expiration = position + start;
            if (expiration < length) {
                next_expiring[position] = expiration_head[expiration];
                expiration_head[expiration] = position;
            }
        }
    };

    for (int column = 0; column < n; ++column) {
        process_diagonal(0, column, n - column);
    }
    for (int row = 1; row < n; ++row) {
        process_diagonal(row, 0, n - row);
    }

    for (const std::int64_t count : answer) {
        std::printf("%lld\n", static_cast<long long>(count));
    }
}
