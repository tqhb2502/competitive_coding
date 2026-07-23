#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Một đường thẳng y = slope*x + intercept.
struct Line {
    long long slope = 0;
    long long intercept = 0;

    long long value(const int x) const {
        return slope * x + intercept;
    }
};

// Li Chao tree đặt trên TRỤC VỊ TRÍ [0, maximum_x]; mỗi nút giữ tối đa một đường.
class SegmentLiChaoTree {
public:
    explicit SegmentLiChaoTree(const int maximum_x)
        : maximum_x_(maximum_x),
          lines_(static_cast<std::size_t>(4 * (maximum_x + 1))),
          occupied_(static_cast<std::size_t>(4 * (maximum_x + 1)), false) {}

    // Thêm đường active trên đoạn vị trí [query_left, query_right].
    void insert(const Line line, const int query_left, const int query_right) {
        insert_segment(
            line, query_left, query_right, 1, 0, maximum_x_
        );
    }

    // Truy vấn max của mọi đường active tại vị trí x.
    std::pair<bool, long long> query(const int x) const {
        return query(x, 1, 0, maximum_x_);
    }

private:
    // Li Chao insert: chèn đường vào subtree gốc node phủ đoạn [left, right].
    void insert_line(Line incoming, const int node, const int left,
                     const int right) {
        // Nút trống: đặt luôn đường vào đây.
        if (!occupied_[static_cast<std::size_t>(node)]) {
            occupied_[static_cast<std::size_t>(node)] = true;
            lines_[static_cast<std::size_t>(node)] = incoming;
            return;
        }

        // So sánh tại điểm giữa: giữ đường tốt hơn ở nút, chuẩn bị đẩy đường thua.
        const int middle = (left + right) / 2;
        const bool better_left =
            incoming.value(left) > lines_[static_cast<std::size_t>(node)].value(left);
        const bool better_middle =
            incoming.value(middle) > lines_[static_cast<std::size_t>(node)].value(middle);
        if (better_middle) {
            std::swap(incoming, lines_[static_cast<std::size_t>(node)]);
        }
        if (left == right) {
            return;
        }
        // Đẩy đường thua xuống nửa mà nó còn có thể vượt (quyết định ở đầu mút trái).
        if (better_left != better_middle) {
            insert_line(incoming, 2 * node, left, middle);
        } else {
            insert_line(incoming, 2 * node + 1, middle + 1, right);
        }
    }

    // Phân rã [query_left, query_right] thành các nút chuẩn rồi Li Chao insert từng nút.
    void insert_segment(const Line line, const int query_left,
                        const int query_right, const int node,
                        const int left, const int right) {
        if (query_right < left || right < query_left) {
            return;
        }
        // Nút nằm gọn trong [l, r]: đây là nút chuẩn, chèn cả đường vào subtree của nó.
        if (query_left <= left && right <= query_right) {
            insert_line(line, node, left, right);
            return;
        }
        const int middle = (left + right) / 2;
        insert_segment(
            line, query_left, query_right, 2 * node, left, middle
        );
        insert_segment(
            line, query_left, query_right, 2 * node + 1, middle + 1, right
        );
    }

    // Đi từ gốc xuống lá x, lấy max giá trị các đường lưu trên đường đi root -> leaf.
    std::pair<bool, long long> query(const int x, const int node,
                                     const int left, const int right) const {
        bool found = occupied_[static_cast<std::size_t>(node)];
        long long best = found
            ? lines_[static_cast<std::size_t>(node)].value(x)
            : 0;
        if (left == right) {
            return {found, best};
        }

        const int middle = (left + right) / 2;
        const auto child = x <= middle
            ? query(x, 2 * node, left, middle)
            : query(x, 2 * node + 1, middle + 1, right);
        if (!found) {
            return child;
        }
        if (!child.first) {
            return {true, best};
        }
        return {true, std::max(best, child.second)};
    }

    int maximum_x_;
    std::vector<Line> lines_;
    std::vector<char> occupied_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int MAXIMUM_X = 100'000;
    int query_count;
    std::cin >> query_count;
    SegmentLiChaoTree tree(MAXIMUM_X);
    while (query_count-- > 0) {
        int type;
        std::cin >> type;
        if (type == 1) {
            // Loại 1: thêm đường y = a*x + b active trên [l, r].
            Line line;
            int left, right;
            std::cin >> line.slope >> line.intercept >> left >> right;
            tree.insert(line, left, right);
        } else {
            // Loại 2: in max tại x, hoặc "NO" nếu không có đường active.
            int x;
            std::cin >> x;
            const auto answer = tree.query(x);
            if (answer.first) {
                std::cout << answer.second << '\n';
            } else {
                std::cout << "NO\n";
            }
        }
    }
    return 0;
}
