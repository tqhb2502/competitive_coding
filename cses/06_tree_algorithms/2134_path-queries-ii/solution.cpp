#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

class MaximumSegmentTree {
public:
    explicit MaximumSegmentTree(const int count) {
        while (size_ < count) {
            size_ *= 2;
        }
        tree_.assign(2 * size_, 0);
    }

    void set_leaf(const int position, const int value) {
        tree_[size_ + position] = value;
    }

    void build() {
        for (int index = size_ - 1; index > 0; --index) {
            tree_[index] = std::max(tree_[2 * index], tree_[2 * index + 1]);
        }
    }

    void assign(const int position, const int value) {
        int index = size_ + position;
        tree_[index] = value;
        for (index /= 2; index > 0; index /= 2) {
            tree_[index] = std::max(tree_[2 * index], tree_[2 * index + 1]);
        }
    }

    int query(int left, int right) const {
        int result = 0;
        left += size_;
        right += size_;
        while (left < right) {
            if ((left & 1) != 0) {
                result = std::max(result, tree_[left++]);
            }
            if ((right & 1) != 0) {
                result = std::max(result, tree_[--right]);
            }
            left /= 2;
            right /= 2;
        }
        return result;
    }

private:
    int size_ = 1;
    std::vector<int> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<int> value(n + 1);
    for (int node = 1; node <= n; ++node) {
        std::cin >> value[node];
    }

    std::vector<std::vector<int>> graph(n + 1);
    for (int edge = 0; edge < n - 1; ++edge) {
        int first, second;
        std::cin >> first >> second;
        graph[first].push_back(second);
        graph[second].push_back(first);
    }

    std::vector<int> parent(n + 1, 0);
    std::vector<int> depth(n + 1, 0);
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> traversal{1};
    parent[1] = -1;
    while (!traversal.empty()) {
        const int node = traversal.back();
        traversal.pop_back();
        order.push_back(node);
        for (const int next : graph[node]) {
            if (next == parent[node]) {
                continue;
            }
            parent[next] = node;
            depth[next] = depth[node] + 1;
            traversal.push_back(next);
        }
    }

    std::vector<int> subtree_size(n + 1, 1);
    std::vector<int> heavy_child(n + 1, 0);
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int node = *iterator;
        for (const int next : graph[node]) {
            if (parent[next] == node) {
                subtree_size[node] += subtree_size[next];
                if (heavy_child[node] == 0 ||
                    subtree_size[next] > subtree_size[heavy_child[node]]) {
                    heavy_child[node] = next;
                }
            }
        }
    }

    std::vector<int> chain_head(n + 1, 0);
    std::vector<int> position(n + 1, 0);
    std::vector<std::pair<int, int>> chain_starts{{1, 1}};
    int timer = 0;
    while (!chain_starts.empty()) {
        auto [node, head] = chain_starts.back();
        chain_starts.pop_back();

        while (node != 0) {
            chain_head[node] = head;
            position[node] = timer++;
            for (const int next : graph[node]) {
                if (parent[next] == node && next != heavy_child[node]) {
                    chain_starts.emplace_back(next, next);
                }
            }
            node = heavy_child[node];
        }
    }

    MaximumSegmentTree segment_tree(n);
    for (int node = 1; node <= n; ++node) {
        segment_tree.set_leaf(position[node], value[node]);
    }
    segment_tree.build();

    while (q-- > 0) {
        int type, first, second;
        std::cin >> type >> first >> second;
        if (type == 1) {
            segment_tree.assign(position[first], second);
            continue;
        }

        int answer = 0;
        int left_node = first;
        int right_node = second;
        while (chain_head[left_node] != chain_head[right_node]) {
            if (depth[chain_head[left_node]] < depth[chain_head[right_node]]) {
                std::swap(left_node, right_node);
            }
            answer = std::max(
                answer,
                segment_tree.query(
                    position[chain_head[left_node]], position[left_node] + 1
                )
            );
            left_node = parent[chain_head[left_node]];
        }
        if (depth[left_node] > depth[right_node]) {
            std::swap(left_node, right_node);
        }
        answer = std::max(
            answer,
            segment_tree.query(
                position[left_node], position[right_node] + 1
            )
        );
        std::cout << answer << ' ';
    }
    std::cout << '\n';
    return 0;
}
