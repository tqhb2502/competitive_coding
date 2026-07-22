#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

class SegmentTree {
public:
    explicit SegmentTree(int size)
        : size_(size), minimum_(static_cast<std::size_t>(4 * size), 0),
          maximum_(static_cast<std::size_t>(4 * size), 0),
          lazy_(static_cast<std::size_t>(4 * size), 0) {}

    void addPrefix(int right, int value) {
        add(1, 1, size_, 1, right, value);
    }

    int minimum() const {
        return minimum_[1];
    }

    int maximum() const {
        return maximum_[1];
    }

private:
    void apply(int node, int value) {
        minimum_[static_cast<std::size_t>(node)] += value;
        maximum_[static_cast<std::size_t>(node)] += value;
        lazy_[static_cast<std::size_t>(node)] += value;
    }

    void push(int node) {
        const int value = lazy_[static_cast<std::size_t>(node)];
        if (value != 0) {
            apply(node * 2, value);
            apply(node * 2 + 1, value);
            lazy_[static_cast<std::size_t>(node)] = 0;
        }
    }

    void add(int node, int left, int right, int queryLeft, int queryRight, int value) {
        if (queryLeft <= left && right <= queryRight) {
            apply(node, value);
            return;
        }
        push(node);
        const int middle = left + (right - left) / 2;
        if (queryLeft <= middle) {
            add(node * 2, left, middle, queryLeft, queryRight, value);
        }
        if (queryRight > middle) {
            add(node * 2 + 1, middle + 1, right, queryLeft, queryRight, value);
        }
        minimum_[static_cast<std::size_t>(node)] =
            std::min(minimum_[static_cast<std::size_t>(node * 2)],
                     minimum_[static_cast<std::size_t>(node * 2 + 1)]);
        maximum_[static_cast<std::size_t>(node)] =
            std::max(maximum_[static_cast<std::size_t>(node * 2)],
                     maximum_[static_cast<std::size_t>(node * 2 + 1)]);
    }

    int size_;
    std::vector<int> minimum_;
    std::vector<int> maximum_;
    std::vector<int> lazy_;
};

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    SegmentTree tree(n);
    for (int move = 0; move < n; ++move) {
        int coin = 0;
        int stack = 0;
        std::cin >> coin >> stack;
        tree.addPrefix(coin, stack == 1 ? 1 : -1);

        if (tree.minimum() >= 0 && tree.maximum() > 0) {
            std::cout << ">\n";
        } else if (tree.maximum() <= 0 && tree.minimum() < 0) {
            std::cout << "<\n";
        } else {
            std::cout << "?\n";
        }
    }
    return 0;
}
