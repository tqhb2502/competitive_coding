#include <iostream>
#include <vector>

using i64 = long long;

// Mỗi node lưu tổng của đoạn, cùng chỉ số con trái và con phải.
// Node 0 đóng vai trò null (không dùng).
struct Node {
    i64 sum = 0;
    int left = 0;
    int right = 0;
};

// Persistent segment tree với kỹ thuật path copying: mỗi cập nhật một điểm
// chỉ tạo ra O(log n) node mới trên đường đi, các node còn lại được dùng chung
// giữa các phiên bản nên việc sao chép mảng chỉ tốn O(1).
class PersistentSegmentTree {
public:
    PersistentSegmentTree(const std::vector<i64>& values, std::size_t capacity) {
        nodes_.reserve(capacity);
        nodes_.push_back(Node{});  // node 0 = null
        initial_root_ = build(0, static_cast<int>(values.size()) - 1, values);
    }

    [[nodiscard]] int initial_root() const {
        return initial_root_;
    }

    // Cập nhật một điểm, trả về gốc của phiên bản MỚI (gốc cũ giữ nguyên).
    int update(int root, int position, i64 value, int n) {
        return update(root, 0, n - 1, position, value);
    }

    // Tổng đoạn [query_left, query_right] trên cây có gốc root.
    [[nodiscard]] i64 query(int root, int query_left, int query_right, int n) const {
        return query(root, 0, n - 1, query_left, query_right);
    }

private:
    std::vector<Node> nodes_;
    int initial_root_ = 0;

    // Cấp phát một node mới trong mảng phẳng, trả về chỉ số của nó.
    int make_node(const Node& node) {
        nodes_.push_back(node);
        return static_cast<int>(nodes_.size()) - 1;
    }

    // Build segment tree ban đầu trên đoạn [lo, hi], cộng tổng từ dưới lên.
    int build(int lo, int hi, const std::vector<i64>& values) {
        if (lo == hi) {
            return make_node(Node{values[static_cast<std::size_t>(lo)], 0, 0});
        }
        const int mid = lo + (hi - lo) / 2;
        const int left = build(lo, mid, values);
        const int right = build(mid + 1, hi, values);
        return make_node(Node{nodes_[static_cast<std::size_t>(left)].sum +
                                  nodes_[static_cast<std::size_t>(right)].sum,
                              left,
                              right});
    }

    // Path copying: tạo node mới sao chép từ node cũ, chỉ đi tiếp xuống nhánh
    // chứa vị trí cần sửa; nhánh còn lại dùng chung con của node cũ.
    int update(int old_node, int lo, int hi, int position, i64 value) {
        const int new_node = make_node(nodes_[static_cast<std::size_t>(old_node)]);
        if (lo == hi) {
            nodes_[static_cast<std::size_t>(new_node)].sum = value;
            return new_node;
        }

        const int mid = lo + (hi - lo) / 2;
        if (position <= mid) {
            const int child = update(nodes_[static_cast<std::size_t>(old_node)].left,
                                     lo,
                                     mid,
                                     position,
                                     value);
            nodes_[static_cast<std::size_t>(new_node)].left = child;
        } else {
            const int child = update(nodes_[static_cast<std::size_t>(old_node)].right,
                                     mid + 1,
                                     hi,
                                     position,
                                     value);
            nodes_[static_cast<std::size_t>(new_node)].right = child;
        }

        // Cập nhật lại tổng của node mới từ hai con của nó.
        const Node& current = nodes_[static_cast<std::size_t>(new_node)];
        nodes_[static_cast<std::size_t>(new_node)].sum =
            nodes_[static_cast<std::size_t>(current.left)].sum +
            nodes_[static_cast<std::size_t>(current.right)].sum;
        return new_node;
    }

    // Duyệt đệ quy: node phủ trọn trong đoạn truy vấn thì cộng thẳng tổng,
    // ngược lại tách xuống hai con.
    i64 query(int node, int lo, int hi, int query_left, int query_right) const {
        if (query_left <= lo && hi <= query_right) {
            return nodes_[static_cast<std::size_t>(node)].sum;
        }
        const int mid = lo + (hi - lo) / 2;
        i64 answer = 0;
        if (query_left <= mid) {
            answer += query(nodes_[static_cast<std::size_t>(node)].left,
                            lo,
                            mid,
                            query_left,
                            query_right);
        }
        if (query_right > mid) {
            answer += query(nodes_[static_cast<std::size_t>(node)].right,
                            mid + 1,
                            hi,
                            query_left,
                            query_right);
        }
        return answer;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;
    std::vector<i64> values(static_cast<std::size_t>(n));
    for (i64& value : values) {
        std::cin >> value;
    }

    // Số node tối đa: build ~ (2n-1) node, mỗi cập nhật thêm <= ~20 node.
    const std::size_t capacity =
        static_cast<std::size_t>(2 * n + 20 * q + 10);
    PersistentSegmentTree tree(values, capacity);

    // roots[k] = gốc của mảng thứ k (đánh chỉ số từ 1); roots[0] không dùng.
    std::vector<int> roots;
    roots.reserve(static_cast<std::size_t>(q + 2));
    roots.push_back(0);
    roots.push_back(tree.initial_root());

    while (q-- > 0) {
        int type = 0;
        int array_index = 0;
        std::cin >> type >> array_index;
        if (type == 1) {
            // Cập nhật một điểm: gán lại roots[k] bằng phiên bản mới.
            int position = 0;
            i64 value = 0;
            std::cin >> position >> value;
            roots[static_cast<std::size_t>(array_index)] =
                tree.update(roots[static_cast<std::size_t>(array_index)], position - 1, value, n);
        } else if (type == 2) {
            // Tổng đoạn trên mảng k.
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            std::cout << tree.query(roots[static_cast<std::size_t>(array_index)],
                                    left - 1,
                                    right - 1,
                                    n)
                      << '\n';
        } else {
            // Sao chép mảng k: mảng mới dùng chung gốc, tốn O(1).
            roots.push_back(roots[static_cast<std::size_t>(array_index)]);
        }
    }
}
