#include <algorithm>
#include <iostream>
#include <vector>

// Một đường thẳng y = slope * x + intercept.
struct Line {
    long long slope = 0;
    long long intercept = 0;

    // Giá trị của đường tại vị trí x (số nguyên chính xác, dùng long long).
    long long value(const int x) const {
        return slope * x + intercept;
    }
};

// Li Chao Tree (bản lấy MAX) trên miền vị trí nguyên [0, maximum_x].
class LiChaoTree {
public:
    explicit LiChaoTree(const int maximum_x)
        : maximum_x_(maximum_x),
          lines_(static_cast<std::size_t>(4 * (maximum_x + 1))),
          occupied_(static_cast<std::size_t>(4 * (maximum_x + 1)), false) {}

    void insert(const Line line) {
        insert(line, 1, 0, maximum_x_);
    }

    long long query(const int x) const {
        return query(x, 1, 0, maximum_x_);
    }

private:
    // Chèn một đường: mỗi lần chỉ đi một nhánh từ gốc xuống lá -> O(log C).
    void insert(Line incoming, const int node, const int left, const int right) {
        // Node còn "rỗng": lưu thẳng đường mới rồi dừng.
        if (!occupied_[static_cast<std::size_t>(node)]) {
            occupied_[static_cast<std::size_t>(node)] = true;
            lines_[static_cast<std::size_t>(node)] = incoming;
            return;
        }

        const int middle = (left + right) / 2;
        // So sánh đường mới với đường đang giữ tại biên trái và tại trung điểm.
        const bool better_left =
            incoming.value(left) > lines_[static_cast<std::size_t>(node)].value(left);
        const bool better_middle =
            incoming.value(middle) > lines_[static_cast<std::size_t>(node)].value(middle);
        // Đường thắng tại trung điểm được giữ lại ở node, đường thua bị đẩy xuống.
        if (better_middle) {
            std::swap(incoming, lines_[static_cast<std::size_t>(node)]);
        }
        if (left == right) {
            return;
        }
        // Hai đường giao nhau ở đúng một phía của middle: better_left khác
        // better_middle nghĩa là đường thua trội hơn ở nửa trái -> đi con trái.
        if (better_left != better_middle) {
            insert(incoming, 2 * node, left, middle);
        } else {
            insert(incoming, 2 * node + 1, middle + 1, right);
        }
    }

    // Truy vấn max f(x): lấy max các đường lưu dọc đường đi từ gốc xuống lá x.
    long long query(const int x, const int node, const int left,
                    const int right) const {
        long long best = occupied_[static_cast<std::size_t>(node)]
            ? lines_[static_cast<std::size_t>(node)].value(x)
            : -(1LL << 62);
        if (left == right) {
            return best;
        }
        const int middle = (left + right) / 2;
        // Đi xuống con chứa x rồi gộp kết quả lại.
        if (x <= middle) {
            best = std::max(best, query(x, 2 * node, left, middle));
        } else {
            best = std::max(best, query(x, 2 * node + 1, middle + 1, right));
        }
        return best;
    }

    int maximum_x_;
    std::vector<Line> lines_;
    std::vector<char> occupied_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Miền của x cố định là [0, 100000] theo ràng buộc đề bài.
    constexpr int MAXIMUM_X = 100'000;
    int query_count;
    std::cin >> query_count;
    LiChaoTree tree(MAXIMUM_X);
    while (query_count-- > 0) {
        int type;
        std::cin >> type;
        if (type == 1) {
            // "1 a b": thêm đường y = a*x + b.
            Line line;
            std::cin >> line.slope >> line.intercept;
            tree.insert(line);
        } else {
            // "2 x": in giá trị lớn nhất của các đường tại x.
            int x;
            std::cin >> x;
            std::cout << tree.query(x) << '\n';
        }
    }
    return 0;
}
