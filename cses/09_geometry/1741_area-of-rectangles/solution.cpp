#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

// Sự kiện dọc của đường quét: tại hoành độ x, thêm/bớt (delta = +1/-1) độ phủ
// trên đoạn y [y_low, y_high).
struct Event {
    int x;
    int y_low;
    int y_high;
    int delta;
};

// Segment tree trên các toạ độ y đã coordinate-compress, duy trì tổng độ dài y
// đang bị phủ bởi các hình chữ nhật active.
class CoverageTree {
public:
    explicit CoverageTree(const std::vector<int>& coordinates)
        : ys_(coordinates), interval_count_(static_cast<int>(coordinates.size()) - 1),
          count_(static_cast<std::size_t>(4 * interval_count_), 0),
          covered_(static_cast<std::size_t>(4 * interval_count_), 0) {}

    // Range update +delta/-delta trên các elementary interval [query_left, query_right].
    void update(int query_left, int query_right, int delta) {
        update(1, 0, interval_count_ - 1, query_left, query_right, delta);
    }

    // Tổng độ dài y đang phủ toàn cục = cover tại gốc (node 1).
    [[nodiscard]] std::int64_t covered_length() const {
        return covered_[1];
    }

private:
    const std::vector<int>& ys_;
    int interval_count_;
    std::vector<int> count_;       // cnt[node]: số hình chữ nhật phủ trọn đoạn node (lazy, không push-down)
    std::vector<std::int64_t> covered_;  // cover[node]: độ dài phủ trong đoạn node

    // Công thức truy hồi cục bộ cho cover.
    void pull(int node, int left, int right) {
        if (count_[static_cast<std::size_t>(node)] > 0) {
            // cnt > 0: cả đoạn bị phủ trọn -> cover = span vật lý theo y.
            covered_[static_cast<std::size_t>(node)] =
                static_cast<std::int64_t>(ys_[static_cast<std::size_t>(right + 1)]) -
                ys_[static_cast<std::size_t>(left)];
        } else if (left == right) {
            // Lá và cnt == 0: không phủ.
            covered_[static_cast<std::size_t>(node)] = 0;
        } else {
            // Còn lại: gộp từ hai con.
            covered_[static_cast<std::size_t>(node)] =
                covered_[static_cast<std::size_t>(2 * node)] +
                covered_[static_cast<std::size_t>(2 * node + 1)];
        }
    }

    void update(int node, int left, int right, int query_left, int query_right, int delta) {
        if (query_left <= left && right <= query_right) {
            // Đoạn node nằm trọn trong khoảng cập nhật: chỉ đổi cnt rồi tính lại cover.
            count_[static_cast<std::size_t>(node)] += delta;
            pull(node, left, right);
            return;
        }

        const int middle = left + (right - left) / 2;
        if (query_left <= middle) {
            update(2 * node, left, middle, query_left, query_right, delta);
        }
        if (query_right > middle) {
            update(2 * node + 1, middle + 1, right, query_left, query_right, delta);
        }
        pull(node, left, right);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int rectangle_count = 0;
    std::cin >> rectangle_count;

    std::vector<Event> events;
    events.reserve(static_cast<std::size_t>(2 * rectangle_count));
    std::vector<int> ys;
    ys.reserve(static_cast<std::size_t>(2 * rectangle_count));

    // Mỗi hình chữ nhật sinh hai sự kiện dọc: +1 tại cạnh trái x1, -1 tại cạnh phải x2.
    for (int i = 0; i < rectangle_count; ++i) {
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
        std::cin >> x1 >> y1 >> x2 >> y2;
        events.push_back({x1, y1, y2, 1});
        events.push_back({x2, y1, y2, -1});
        ys.push_back(y1);
        ys.push_back(y2);
    }

    // Coordinate compression trục y và sắp các sự kiện theo x tăng dần.
    std::sort(ys.begin(), ys.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
    std::sort(events.begin(), events.end(), [](const Event& first, const Event& second) {
        return first.x < second.x;
    });

    CoverageTree tree(ys);
    std::int64_t area = 0;
    int previous_x = events.front().x;
    std::size_t event_index = 0;

    // Quét từng nhóm sự kiện cùng hoành độ.
    while (event_index < events.size()) {
        const int x = events[event_index].x;
        // Cộng diện tích dải [previous_x, x] TRƯỚC khi áp dụng sự kiện tại x hiện tại.
        area += static_cast<std::int64_t>(x - previous_x) * tree.covered_length();

        // Áp dụng mọi sự kiện có cùng hoành độ x.
        while (event_index < events.size() && events[event_index].x == x) {
            const Event& event = events[event_index];
            // Ánh xạ [y_low, y_high) sang chỉ số elementary interval [left, right].
            const int left = static_cast<int>(
                std::lower_bound(ys.begin(), ys.end(), event.y_low) - ys.begin());
            const int right = static_cast<int>(
                std::lower_bound(ys.begin(), ys.end(), event.y_high) - ys.begin()) - 1;
            tree.update(left, right, event.delta);
            ++event_index;
        }
        previous_x = x;
    }

    std::cout << area << '\n';
    return 0;
}
