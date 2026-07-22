#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

struct Node {
    int left = 0;
    int right = 0;
    long long sum = 0;
};

class PersistentSegmentTree {
public:
    PersistentSegmentTree(const int value_count, const int item_count)
        : value_count_(value_count) {
        int levels = 1;
        for (int width = 1; width < value_count_; width <<= 1) {
            ++levels;
        }
        nodes_.reserve(
            static_cast<std::size_t>(item_count) * levels + 1U
        );
        nodes_.push_back(Node{});
    }

    int insert(const int previous_root, const int position,
               const long long value) {
        return insert(previous_root, 0, value_count_ - 1, position, value);
    }

    const Node& operator[](const int index) const {
        return nodes_[index];
    }

private:
    int value_count_;
    std::vector<Node> nodes_;

    int insert(const int previous, const int low, const int high,
               const int position, const long long value) {
        const int current = static_cast<int>(nodes_.size());
        nodes_.push_back(nodes_[previous]);
        nodes_[current].sum += value;

        if (low == high) {
            return current;
        }

        const int middle = low + (high - low) / 2;
        if (position <= middle) {
            nodes_[current].left = insert(
                nodes_[previous].left, low, middle, position, value
            );
        } else {
            nodes_[current].right = insert(
                nodes_[previous].right, middle + 1, high, position, value
            );
        }
        return current;
    }
};

struct TraversalFrame {
    int before;
    int after;
    int low;
    int high;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<long long> coins(n + 1);
    std::vector<long long> values;
    values.reserve(n);
    for (int index = 1; index <= n; ++index) {
        std::cin >> coins[index];
        values.push_back(coins[index]);
    }

    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());

    PersistentSegmentTree tree(static_cast<int>(values.size()), n);
    std::vector<int> roots(n + 1, 0);
    for (int index = 1; index <= n; ++index) {
        const int position = static_cast<int>(
            std::lower_bound(values.begin(), values.end(), coins[index]) -
            values.begin()
        );
        roots[index] = tree.insert(roots[index - 1], position, coins[index]);
    }

    std::vector<TraversalFrame> stack;
    stack.reserve(64);
    while (q-- > 0) {
        int first, last;
        std::cin >> first >> last;

        long long reachable = 0;
        stack.clear();
        stack.push_back({
            roots[first - 1], roots[last], 0,
            static_cast<int>(values.size()) - 1
        });

        while (!stack.empty()) {
            const TraversalFrame frame = stack.back();
            stack.pop_back();

            const long long subtree_sum =
                tree[frame.after].sum - tree[frame.before].sum;
            if (subtree_sum == 0) {
                continue;
            }
            if (values[frame.low] > reachable + 1) {
                break;
            }
            if (values[frame.high] <= reachable + 1) {
                reachable += subtree_sum;
                continue;
            }

            const int middle = frame.low + (frame.high - frame.low) / 2;
            stack.push_back({
                tree[frame.before].right,
                tree[frame.after].right,
                middle + 1,
                frame.high
            });
            stack.push_back({
                tree[frame.before].left,
                tree[frame.after].left,
                frame.low,
                middle
            });
        }

        std::cout << reachable + 1 << '\n';
    }
    return 0;
}
