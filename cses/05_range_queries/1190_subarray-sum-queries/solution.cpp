// Subarray Sum Queries - CSES 1190
// Segment tree nodes store total, best prefix/suffix, and best subarray sum.

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

struct Node {
    long long total = 0;
    long long prefix = 0;
    long long suffix = 0;
    long long best = 0;
};

Node merge_nodes(const Node& left, const Node& right) {
    Node result;
    result.total = left.total + right.total;
    result.prefix = std::max(left.prefix, left.total + right.prefix);
    result.suffix = std::max(right.suffix, right.total + left.suffix);
    result.best = std::max({left.best, right.best, left.suffix + right.prefix});
    return result;
}

Node make_leaf(long long value) {
    const long long nonnegative = std::max(0LL, value);
    return {value, nonnegative, nonnegative, nonnegative};
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t m = 0;
    std::cin >> n >> m;

    std::size_t size = 1;
    while (size < n) {
        size *= 2;
    }
    std::vector<Node> tree(2 * size);
    for (std::size_t i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        tree[size + i] = make_leaf(value);
    }
    for (std::size_t node = size; node-- > 1;) {
        tree[node] = merge_nodes(tree[2 * node], tree[2 * node + 1]);
    }

    for (std::size_t update = 0; update < m; ++update) {
        std::size_t position = 0;
        long long value = 0;
        std::cin >> position >> value;

        std::size_t node = size + position - 1;
        tree[node] = make_leaf(value);
        for (node /= 2; node >= 1; node /= 2) {
            tree[node] = merge_nodes(tree[2 * node], tree[2 * node + 1]);
        }
        std::cout << tree[1].best << '\n';
    }
    return 0;
}
