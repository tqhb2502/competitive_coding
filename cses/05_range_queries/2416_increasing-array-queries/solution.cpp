#include <iostream>
#include <utility>
#include <vector>

// Fenwick / BIT hỗ trợ range update + range sum bằng kỹ thuật 2 BIT:
// một BIT cho hệ số (coefficient), một BIT cho phần bù (offset).
class RangeAddRangeSumFenwick {
public:
    explicit RangeAddRangeSumFenwick(const int size)
        : size_(size), coefficient_(size + 2, 0), offset_(size + 2, 0) {}

    // Cộng value vào mọi vị trí trong [left, right].
    void add_range(const int left, const int right, const long long value) {
        if (left > right) {
            return;
        }
        add(coefficient_, left, value);
        add(coefficient_, right + 1, -value);
        add(offset_, left, value * (left - 1));
        add(offset_, right + 1, -value * right);
    }

    // Tổng các giá trị trong [left, right].
    long long range_sum(const int left, const int right) const {
        return prefix_sum(right) - prefix_sum(left - 1);
    }

private:
    int size_;
    std::vector<long long> coefficient_;
    std::vector<long long> offset_;

    void add(std::vector<long long>& bit, int index, const long long value) {
        while (index <= size_) {
            bit[index] += value;
            index += index & -index;
        }
    }

    long long sum(const std::vector<long long>& bit, int index) const {
        long long result = 0;
        while (index > 0) {
            result += bit[index];
            index -= index & -index;
        }
        return result;
    }

    // prefix(i) = B1.query(i) * i - B2.query(i).
    long long prefix_sum(const int index) const {
        return sum(coefficient_, index) * index - sum(offset_, index);
    }
};

// Một đoạn liên tiếp mà mảng ảo g = max(x[L..i]) bằng hằng số value.
struct ConstantSegment {
    long long value;
    int length;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    // Đọc mảng và tính prefix sum của x.
    std::vector<long long> values(n + 1, 0);
    std::vector<long long> prefix(n + 1, 0);
    for (int index = 1; index <= n; ++index) {
        std::cin >> values[index];
        prefix[index] = prefix[index - 1] + values[index];
    }

    // Offline: nhóm các truy vấn theo điểm trái (left) để xử lý một lượt.
    std::vector<std::vector<std::pair<int, int>>> queries_by_left(n + 1);
    for (int query_index = 0; query_index < q; ++query_index) {
        int left, right;
        std::cin >> left >> right;
        queries_by_left[left].emplace_back(right, query_index);
    }

    std::vector<long long> answers(q, 0);
    // Monotonic stack các đoạn hằng của g = max(x[L..i]); đỉnh là đoạn trái nhất.
    std::vector<ConstantSegment> stack;
    stack.reserve(n);
    // BIT lưu g[i] = max(x[L..i]) cho mọi i >= L (running maximum).
    RangeAddRangeSumFenwick running_maximum(n);

    // Duyệt điểm trái L từ n về 1, thêm dần x[L] vào bên trái.
    for (int left = n; left >= 1; --left) {
        const long long value = values[left];
        int next_position = left + 1;
        int merged_length = 0;

        // Pop các đoạn trái nhất có value <= x[L] và nâng chúng lên thành x[L].
        while (!stack.empty() && stack.back().value <= value) {
            const ConstantSegment segment = stack.back();
            stack.pop_back();
            const int segment_end = next_position + segment.length - 1;
            running_maximum.add_range(
                next_position, segment_end, value - segment.value
            );
            next_position = segment_end + 1;
            merged_length += segment.length;
        }

        // Thêm vị trí L với giá trị x[L] rồi đẩy đoạn mới vừa gộp vào stack.
        running_maximum.add_range(left, left, value);
        stack.push_back({value, merged_length + 1});

        // Trả lời các truy vấn có điểm trái đúng bằng L: g đang chính xác trên [L, b].
        for (const auto& [right, query_index] : queries_by_left[left]) {
            const long long maximum_sum =
                running_maximum.range_sum(left, right);
            const long long original_sum = prefix[right] - prefix[left - 1];
            answers[query_index] = maximum_sum - original_sum;
        }
    }

    for (const long long answer : answers) {
        std::cout << answer << '\n';
    }
    return 0;
}
