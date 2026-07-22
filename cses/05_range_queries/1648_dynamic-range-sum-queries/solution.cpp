// Dynamic Range Sum Queries - CSES 1648
// Fenwick tree for point assignments (as deltas) and inclusive range sums.

#include <cstddef>
#include <iostream>
#include <vector>

class FenwickTree {
public:
    explicit FenwickTree(std::size_t size) : tree_(size + 1, 0) {}

    void add(std::size_t position, long long delta) {
        const std::size_t size = tree_.size() - 1;
        for (std::size_t i = position; i <= size; i += i & -i) {
            tree_[i] += delta;
        }
    }

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
            sums.add(first, second - current[first]);
            current[first] = second;
        } else {
            const std::size_t right = static_cast<std::size_t>(second);
            std::cout << sums.prefix_sum(right) - sums.prefix_sum(first - 1) << '\n';
        }
    }
    return 0;
}
