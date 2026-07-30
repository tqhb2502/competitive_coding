#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

// Segment tree lazy: cộng thêm một giá trị cho cả đoạn tiền tố và
// truy vấn nhanh min/max của toàn bộ mảng S.
class SegmentTree {
public:
    explicit SegmentTree(int size)
        : size_(size), minimum_(static_cast<std::size_t>(4 * size), 0),
          maximum_(static_cast<std::size_t>(4 * size), 0),
          lazy_(static_cast<std::size_t>(4 * size), 0) {}

    // Cộng value cho mọi S_j với j trong đoạn [1, right].
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
    // Ghi nhận phần cộng value lên toàn bộ nút hiện tại.
    void apply(int node, int value) {
        minimum_[static_cast<std::size_t>(node)] += value;
        maximum_[static_cast<std::size_t>(node)] += value;
        lazy_[static_cast<std::size_t>(node)] += value;
    }

    // Đẩy lazy xuống hai nút con trước khi đi tiếp.
    void push(int node) {
        const int value = lazy_[static_cast<std::size_t>(node)];
        if (value != 0) {
            apply(node * 2, value);
            apply(node * 2 + 1, value);
            lazy_[static_cast<std::size_t>(node)] = 0;
        }
    }

    void add(int node, int left, int right, int queryLeft, int queryRight, int value) {
        // Đoạn của nút nằm trọn trong đoạn cần cộng: cập nhật lazy rồi dừng.
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
        // Gộp lại min/max từ hai nút con.
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
    // Cây quản lý mảng S_j = tổng dấu d_i với i >= j (các hệ số của D theo y_j).
    SegmentTree tree(n);
    for (int move = 0; move < n; ++move) {
        int coin = 0;
        int stack = 0;
        std::cin >> coin >> stack;
        // Đặt xu coin: ngăn trái (stack == 1) mang dấu +1, ngăn phải mang dấu -1.
        // Việc này cộng dấu đó vào mọi S_j với j <= coin.
        tree.addPrefix(coin, stack == 1 ? 1 : -1);

        // Mọi y_j dương nên dấu chắc chắn của D được quyết định bởi min/max của S.
        if (tree.minimum() >= 0 && tree.maximum() > 0) {
            std::cout << ">\n";  // Mọi S_j >= 0 và có S_j > 0: ngăn trái nặng hơn.
        } else if (tree.maximum() <= 0 && tree.minimum() < 0) {
            std::cout << "<\n";  // Mọi S_j <= 0 và có S_j < 0: ngăn phải nặng hơn.
        } else {
            std::cout << "?\n";  // Còn lại: chưa thể kết luận.
        }
    }
    return 0;
}
