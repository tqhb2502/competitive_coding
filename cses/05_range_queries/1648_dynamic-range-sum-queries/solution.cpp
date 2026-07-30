#include <cstddef>
#include <iostream>
#include <vector>

// Fenwick tree (BIT) 1-indexed: cập nhật một điểm và prefix-sum đều O(log n).
class FenwickTree {
public:
    explicit FenwickTree(std::size_t size) : tree_(size + 1, 0) {}

    // Cộng delta vào vị trí position, đi lên theo i += i & (-i).
    void add(std::size_t position, long long delta) {
        const std::size_t size = tree_.size() - 1;
        for (std::size_t i = position; i <= size; i += i & -i) {
            tree_[i] += delta;
        }
    }

    // Prefix sum của đoạn [1, position], đi xuống theo i -= i & (-i).
    [[nodiscard]] long long prefix_sum(std::size_t position) const {
        long long result = 0;
        for (std::size_t i = position; i > 0; i -= i & -i) {
            result += tree_[i];
        }
        return result;
    }

private:
    std::vector<long long> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t q = 0;
    std::cin >> n >> q;

    // current[i] giữ giá trị hiện tại của vị trí i; dựng BIT từ mảng ban đầu.
    FenwickTree sums(n);
    std::vector<long long> current(n + 1, 0);
    for (std::size_t i = 1; i <= n; ++i) {
        std::cin >> current[i];
        sums.add(i, current[i]);
    }

    for (std::size_t query = 0; query < q; ++query) {
        int type = 0;
        std::size_t first = 0;
        long long second = 0;
        std::cin >> type >> first >> second;
        if (type == 1) {
            // Truy vấn "1 k u" là phép GÁN: cập nhật BIT bằng chênh lệch u - current[k].
            sums.add(first, second - current[first]);
            current[first] = second;
        } else {
            // Truy vấn "2 a b": tổng đoạn = prefix_sum(b) - prefix_sum(a - 1).
            const std::size_t right = static_cast<std::size_t>(second);
            std::cout << sums.prefix_sum(right) - sums.prefix_sum(first - 1) << '\n';
        }
    }
    return 0;
}
