#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

// Iterative segment tree (bottom-up) hỗ trợ range-min với point update.
class MinimumSegmentTree {
public:
    explicit MinimumSegmentTree(const std::vector<long long>& values)
        : size_(static_cast<int>(values.size())),
          tree_(2 * size_, infinity()) {
        // Gán giá trị vào các lá rồi dựng min ngược lên gốc.
        for (int index = 0; index < size_; ++index) {
            tree_[size_ + index] = values[index];
        }
        for (int index = size_ - 1; index > 0; --index) {
            tree_[index] = std::min(tree_[2 * index], tree_[2 * index + 1]);
        }
    }

    // Point update: gán giá trị mới cho một lá và cập nhật ngược lên gốc.
    void assign(const int position, const long long value) {
        int index = size_ + position;
        tree_[index] = value;
        for (index /= 2; index > 0; index /= 2) {
            tree_[index] = std::min(tree_[2 * index], tree_[2 * index + 1]);
        }
    }

    // Truy vấn min trên nửa khoảng [left, right).
    long long query(int left, int right) const {
        long long result = infinity();
        left += size_;
        right += size_;
        while (left < right) {
            if ((left & 1) != 0) {
                result = std::min(result, tree_[left++]);
            }
            if ((right & 1) != 0) {
                result = std::min(result, tree_[--right]);
            }
            left /= 2;
            right /= 2;
        }
        return result;
    }

private:
    static constexpr long long infinity() {
        return std::numeric_limits<long long>::max() / 4;
    }

    int size_;
    std::vector<long long> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    // Tách trị tuyệt đối: L[a] = p_a - a (cho a <= k), R[a] = p_a + a (cho a >= k).
    std::vector<long long> minus_index(n);
    std::vector<long long> plus_index(n);
    for (int position = 1; position <= n; ++position) {
        long long price;
        std::cin >> price;
        minus_index[position - 1] = price - position;
        plus_index[position - 1] = price + position;
    }

    // Cây trái giữ prefix-min của L, cây phải giữ suffix-min của R.
    MinimumSegmentTree left_tree(minus_index);
    MinimumSegmentTree right_tree(plus_index);

    while (q-- > 0) {
        int type, position;
        std::cin >> type >> position;
        if (type == 1) {
            // Cập nhật giá p_position = price ở cả hai cây.
            long long price;
            std::cin >> price;
            left_tree.assign(position - 1, price - position);
            right_tree.assign(position - 1, price + position);
        } else {
            // Đáp án = min( prefix-min L trên [1, k] + k , suffix-min R trên [k, n] - k ).
            const long long from_left =
                left_tree.query(0, position) + position;
            const long long from_right =
                right_tree.query(position - 1, n) - position;
            std::cout << std::min(from_left, from_right) << '\n';
        }
    }
    return 0;
}
