#include <algorithm>
#include <iostream>
#include <vector>

using i64 = long long;

// Mỗi node lưu tổng đoạn (sum) và maximum prefix sum (best_prefix, đã tính cả
// prefix rỗng nên luôn >= 0).
struct Node {
    i64 sum = 0;
    i64 best_prefix = 0;
};

// Hợp nhất hai node con trái/phải, KHÔNG giao hoán: prefix của đoạn ghép hoặc
// nằm trọn trong node trái, hoặc phủ hết node trái rồi lấy thêm một prefix của
// node phải.
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

    // Làm tròn kích thước lên lũy thừa của 2 cho segment tree lặp.
    int size = 1;
    while (size < n) {
        size *= 2;
    }

    // Nạp giá trị vào lá: node (một phần tử x) có sum = x, best_prefix = max(0, x).
    std::vector<Node> tree(static_cast<std::size_t>(2 * size));
    for (int i = 0; i < n; ++i) {
        i64 value = 0;
        std::cin >> value;
        tree[static_cast<std::size_t>(size + i)] = Node{value, std::max<i64>(0, value)};
    }
    // Build bottom-up: mỗi node trong = merge của hai con.
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
            // Point update: gán lại lá rồi đi ngược lên gốc, cập nhật tổ tiên.
            int node = size + first - 1;
            tree[static_cast<std::size_t>(node)] = Node{second, std::max<i64>(0, second)};
            for (node /= 2; node > 0; node /= 2) {
                tree[static_cast<std::size_t>(node)] =
                    merge(tree[static_cast<std::size_t>(node * 2)],
                          tree[static_cast<std::size_t>(node * 2 + 1)]);
            }
        } else {
            // Truy vấn đoạn [a, b]: vì merge không giao hoán nên gom biên trái
            // vào left_result theo thứ tự trái->phải, gom biên phải vào
            // right_result bằng cách prepend, cuối cùng merge hai bên.
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
            // Đáp án là best_prefix của kết quả hợp nhất (luôn >= 0 vì cho prefix rỗng).
            std::cout << merge(left_result, right_result).best_prefix << '\n';
        }
    }
}
