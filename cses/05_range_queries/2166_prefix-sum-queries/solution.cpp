#include <algorithm>
#include <iostream>
#include <vector>

using i64 = long long;

struct Node {
    i64 sum = 0;
    i64 best_prefix = 0;
};

[[nodiscard]] Node merge(const Node& left, const Node& right) {
    return Node{left.sum + right.sum,
                std::max(left.best_prefix, left.sum + right.best_prefix)};
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;
    int size = 1;
    while (size < n) {
        size *= 2;
    }

    std::vector<Node> tree(static_cast<std::size_t>(2 * size));
    for (int i = 0; i < n; ++i) {
        i64 value = 0;
        std::cin >> value;
        tree[static_cast<std::size_t>(size + i)] = Node{value, std::max<i64>(0, value)};
    }
    for (int node = size - 1; node > 0; --node) {
        tree[static_cast<std::size_t>(node)] =
            merge(tree[static_cast<std::size_t>(node * 2)],
                  tree[static_cast<std::size_t>(node * 2 + 1)]);
    }

    while (q-- > 0) {
        int type = 0;
        int first = 0;
        i64 second = 0;
        std::cin >> type >> first >> second;
        if (type == 1) {
            int node = size + first - 1;
            tree[static_cast<std::size_t>(node)] = Node{second, std::max<i64>(0, second)};
            for (node /= 2; node > 0; node /= 2) {
                tree[static_cast<std::size_t>(node)] =
                    merge(tree[static_cast<std::size_t>(node * 2)],
                          tree[static_cast<std::size_t>(node * 2 + 1)]);
            }
        } else {
            int left = size + first - 1;
            int right = size + static_cast<int>(second);
            Node left_result;
            Node right_result;
            while (left < right) {
                if ((left & 1) != 0) {
                    left_result = merge(left_result, tree[static_cast<std::size_t>(left)]);
                    ++left;
                }
                if ((right & 1) != 0) {
                    --right;
                    right_result = merge(tree[static_cast<std::size_t>(right)], right_result);
                }
                left /= 2;
                right /= 2;
            }
            std::cout << merge(left_result, right_result).best_prefix << '\n';
        }
    }
}
