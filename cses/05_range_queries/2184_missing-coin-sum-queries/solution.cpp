#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

// Một node của persistent segment tree: con trái/phải và tổng TIỀN của cây con.
struct Node {
    int left = 0;
    int right = 0;
    long long sum = 0;
};

// Persistent segment tree dựng trên trục GIÁ TRỊ đã nén; mỗi lần chèn tạo ra
// O(log m) node mới nên giữ lại được mọi version theo prefix vị trí.
class PersistentSegmentTree {
public:
    PersistentSegmentTree(const int value_count, const int item_count)
        : value_count_(value_count) {
        // Ước lượng số node tối đa = số lần chèn * chiều cao cây.
        int levels = 1;
        for (int width = 1; width < value_count_; width <<= 1) {
            ++levels;
        }
        nodes_.reserve(
            static_cast<std::size_t>(item_count) * levels + 1U
        );
        nodes_.push_back(Node{});  // node 0 là null (sum = 0)
    }

    // Chèn thêm một đồng xu trị giá value vào vị trí giá trị position, sinh ra
    // một root mới dựa trên root previous_root.
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
        // Sao chép node cũ rồi cộng thêm tiền của đồng xu mới lên đường đi.
        const int current = static_cast<int>(nodes_.size());
        nodes_.push_back(nodes_[previous]);
        nodes_[current].sum += value;

        if (low == high) {
            return current;  // tới lá: giá trị này đã được cập nhật tổng tiền
        }

        // Chỉ tạo node mới trên nhánh chứa position, nhánh còn lại dùng chung.
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

// Một khung của DFS tường minh: cặp node (trước/sau) và khoảng giá trị phủ.
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

    // Nén tọa độ theo giá trị đồng xu.
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());

    // Dựng cây theo prefix vị trí: roots[i] = đã chèn i đồng đầu tiên x_1..x_i.
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

        // reachable = R: mọi tổng trong [0, R] đều tạo được từ đoạn [first, last].
        long long reachable = 0;
        stack.clear();
        // Hiệu hai version root[last] - root[first-1] cho tổng tiền theo giá trị.
        stack.push_back({
            roots[first - 1], roots[last], 0,
            static_cast<int>(values.size()) - 1
        });

        // DFS in-order theo thứ tự giá trị tăng dần, chạy greedy trên đường đi.
        while (!stack.empty()) {
            const TraversalFrame frame = stack.back();
            stack.pop_back();

            const long long subtree_sum =
                tree[frame.after].sum - tree[frame.before].sum;
            if (subtree_sum == 0) {
                continue;  // đoạn giá trị này không có đồng xu nào
            }
            if (values[frame.low] > reachable + 1) {
                break;  // giá trị nhỏ nhất còn lại > R+1 -> R+1 không tạo được
            }
            if (values[frame.high] <= reachable + 1) {
                reachable += subtree_sum;  // gộp cả cây con vào reach một lần
                continue;
            }

            // Node bắc cầu qua ngưỡng R+1: đẩy phải trước, trái sau để pop trái
            // trước (giữ đúng thứ tự in-order tăng dần).
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
