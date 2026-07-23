#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

// Sự kiện của sweep line: tại hoành độ x, loại type (add=0 < query=1 < remove=2).
// Với add/remove: left = rank của y trong BIT. Với query: [left, right) là khoảng rank.
struct Event {
    int x;
    int type;
    int left;
    int right;
};

// BIT (Fenwick tree) trên trục y: point update, prefix sum để đếm range động.
class FenwickTree {
public:
    explicit FenwickTree(int size)
        : size_(size), tree_(static_cast<std::size_t>(size) + 1U, 0) {}

    void add(int index, int value) {
        for (int i = index; i <= size_; i += i & -i) {
            tree_[static_cast<std::size_t>(i)] += value;
        }
    }

    int prefix_sum(int index) const {
        int result = 0;
        for (int i = index; i > 0; i -= i & -i) {
            result += tree_[static_cast<std::size_t>(i)];
        }
        return result;
    }

private:
    int size_;
    std::vector<int> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    struct Horizontal {
        int left_x;
        int right_x;
        int y;
    };
    struct Vertical {
        int x;
        int low_y;
        int high_y;
    };

    std::vector<Horizontal> horizontals;
    std::vector<Vertical> verticals;
    std::vector<int> horizontal_ys;  // các y của đoạn ngang, dùng để nén tọa độ
    horizontals.reserve(static_cast<std::size_t>(n));
    verticals.reserve(static_cast<std::size_t>(n));
    horizontal_ys.reserve(static_cast<std::size_t>(n));

    // Phân loại từng đoạn thành ngang (y1 == y2) hoặc dọc (x1 == x2).
    for (int i = 0; i < n; ++i) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        if (y1 == y2) {
            horizontals.push_back({x1, x2, y1});
            horizontal_ys.push_back(y1);
        } else {
            verticals.push_back({x1, y1, y2});
        }
    }

    // Nén tọa độ y của các đoạn ngang thành rank 1..m.
    std::sort(horizontal_ys.begin(), horizontal_ys.end());
    horizontal_ys.erase(
        std::unique(horizontal_ys.begin(), horizontal_ys.end()),
        horizontal_ys.end());

    std::vector<Event> events;
    events.reserve(2 * horizontals.size() + verticals.size());

    // Mỗi đoạn ngang -> sự kiện add tại xl và remove tại xr (rank 1-based).
    for (const Horizontal& segment : horizontals) {
        const int rank = static_cast<int>(std::lower_bound(
            horizontal_ys.begin(), horizontal_ys.end(), segment.y) -
            horizontal_ys.begin()) + 1;
        events.push_back({segment.left_x, 0, rank, 0});
        events.push_back({segment.right_x, 2, rank, 0});
    }

    // Mỗi đoạn dọc -> sự kiện query lấy khoảng rank [left, right) ứng với [low_y, high_y].
    for (const Vertical& segment : verticals) {
        const int left = static_cast<int>(std::lower_bound(
            horizontal_ys.begin(), horizontal_ys.end(), segment.low_y) -
            horizontal_ys.begin());
        const int right = static_cast<int>(std::upper_bound(
            horizontal_ys.begin(), horizontal_ys.end(), segment.high_y) -
            horizontal_ys.begin());
        if (left < right) {
            events.push_back({segment.x, 1, left, right});
        }
    }

    // Sắp xếp theo x tăng dần, trùng x thì theo type add < query < remove.
    std::sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        return std::tie(a.x, a.type) < std::tie(b.x, b.type);
    });

    // Quét: add/remove cập nhật BIT, query cộng số đoạn ngang đang hoạt động trong khoảng.
    FenwickTree active(static_cast<int>(horizontal_ys.size()));
    long long answer = 0;
    for (const Event& event : events) {
        if (event.type == 0) {
            active.add(event.left, 1);
        } else if (event.type == 1) {
            answer += active.prefix_sum(event.right) -
                      active.prefix_sum(event.left);
        } else {
            active.add(event.left, -1);
        }
    }

    std::cout << answer << '\n';
    return 0;
}
