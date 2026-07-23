#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>

namespace {
// start, finish và màu của một ô được đóng gói vào một số 32 bit:
//   - bit [0, 12)  : start (reach của góc trên-trái)
//   - bit [12, 24) : finish (reach của góc dưới-phải)
//   - bit [24, ...): chỉ số màu (chữ cái)
constexpr int REACH_BITS = 12;
constexpr std::uint32_t REACH_MASK = (1U << REACH_BITS) - 1;
constexpr int FINISH_SHIFT = REACH_BITS;
constexpr int COLOR_SHIFT = 2 * REACH_BITS;

constexpr int WORD_BITS = 64;
constexpr int WORDS_PER_BLOCK = 8;

// Bộ đọc dựa trên fread: input tới ~9 triệu ký tự nên tránh formatted input.
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

    // Đọc một dòng lưới có đúng length ký tự (bỏ qua khoảng trắng đầu dòng).
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

// Tập các điểm bắt đầu "đã hết hiệu lực" trên một đường chéo, lưu bằng bitset
// động kèm bộ đếm theo word (64 vị trí) và theo khối (512 vị trí) để đếm nhanh
// số phần tử trong một đoạn.
class ExpiredStarts {
public:
    explicit ExpiredStarts(int capacity)
        : bits_((capacity + WORD_BITS - 1) / WORD_BITS),
          word_count_(bits_.size()),
          block_count_((bits_.size() + WORDS_PER_BLOCK - 1) /
                       WORDS_PER_BLOCK) {}

    // Xoá cấu trúc để tái sử dụng cho đường chéo dài length.
    void clear(int length) {
        const int words = (length + WORD_BITS - 1) / WORD_BITS;
        const int blocks = (words + WORDS_PER_BLOCK - 1) / WORDS_PER_BLOCK;
        std::fill(bits_.begin(), bits_.begin() + words, 0);
        std::fill(word_count_.begin(), word_count_.begin() + words, 0);
        std::fill(block_count_.begin(), block_count_.begin() + blocks, 0);
        total_ = 0;
    }

    // Đánh dấu một vị trí là đã hết hiệu lực và cập nhật các bộ đếm.
    void add(int position) {
        const int word = position / WORD_BITS;
        bits_[word] |= std::uint64_t{1} << (position % WORD_BITS);
        ++word_count_[word];
        ++block_count_[word / WORDS_PER_BLOCK];
        ++total_;
    }

    // Mọi vị trí đã hết hiệu lực đều nhỏ hơn right. Đếm số vị trí trong
    // [left, right), chọn quét từ phía nào rẻ hơn giữa đếm trực tiếp đoạn và
    // lấy tổng trừ đi phần prefix [0, left).
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

        // Cách 1: đếm trực tiếp các word phủ đoạn [left, right).
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

        // Cách 2: đếm prefix [0, left) rồi lấy total_ trừ đi.
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

    // Quét từ dưới-phải lên trên-trái để tính start[p] = min(số ký tự giống nhau
    // liên tiếp sang phải, số ký tự giống nhau liên tiếp đi xuống) tính từ p.
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

    // Quét từ trên-trái xuống dưới-phải để tính finish[q] = min(số ký tự giống
    // nhau liên tiếp sang trái, số ký tự giống nhau liên tiếp đi lên) tới q.
    // Đồng thời lưu luôn chỉ số màu của ô.
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
    std::vector<int> expiration_head(n + 1, -1);  // bucket theo thời điểm hết hiệu lực
    std::vector<int> next_expiring(n, -1);        // danh sách liên kết trong bucket
    std::vector<std::uint32_t> diagonal(n);
    ExpiredStarts expired(n);

    // Quét một đường chéo chính (đi xuống-phải) bắt đầu tại (first_row, first_column).
    // Với mỗi góc dưới-phải q, finish[q] = g cho g ứng viên p thuộc [q-g+1, q];
    // các ứng viên còn hiệu lực (chưa hết reach) tạo thành hình vuông viền đồng màu.
    const auto process_diagonal = [&](int first_row, int first_column, int length) {
        std::fill(expiration_head.begin(), expiration_head.begin() + length + 1, -1);
        expired.clear(length);

        // Gom dữ liệu của đường chéo vào một mảng liên tục để giảm cache miss.
        std::size_t cell = static_cast<std::size_t>(first_row) * n + first_column;
        const std::size_t diagonal_step = static_cast<std::size_t>(n) + 1;
        for (int position = 0; position < length;
             ++position, cell += diagonal_step) {
            diagonal[position] = cell_info[cell];
        }

        for (int position = 0; position < length; ++position) {
            // Kích hoạt các điểm bắt đầu hết hiệu lực đúng tại vị trí này.
            int expiring = expiration_head[position];
            while (expiring != -1) {
                expired.add(expiring);
                expiring = next_expiring[expiring];
            }

            const std::uint32_t info = diagonal[position];
            const int start = info & REACH_MASK;
            const int finish = (info >> FINISH_SHIFT) & REACH_MASK;
            // g = finish ứng viên p thuộc [left, position]; trừ đi các p đã hết hiệu lực.
            const int left = std::max(0, position - finish + 1);
            const int candidates = position - left + 1;
            const int invalid = expired.count_between(left, position);
            const int color = info >> COLOR_SHIFT;
            answer[color] += candidates - invalid;

            // Lên lịch: điểm này hết hiệu lực tại position + start.
            const int expiration = position + start;
            if (expiration < length) {
                next_expiring[position] = expiration_head[expiration];
                expiration_head[expiration] = position;
            }
        }
    };

    // Các đường chéo bắt đầu từ hàng trên cùng và từ cột trái ngoài cùng.
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
