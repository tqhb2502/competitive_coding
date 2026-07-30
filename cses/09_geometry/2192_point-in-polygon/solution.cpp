#include <algorithm>
#include <iostream>
#include <vector>

struct Point {
    long long x;
    long long y;
};

// Tích có hướng (cross product) của vector a->b với vector a->p.
// Bằng 0 nghĩa là ba điểm thẳng hàng.
long long cross(const Point& a, const Point& b, const Point& p) {
    return (b.x - a.x) * (p.y - a.y) -
           (b.y - a.y) * (p.x - a.x);
}

// P nằm trên đoạn a-b khi thẳng hàng (cross == 0) và nằm trong bounding box.
bool lies_on_segment(const Point& a, const Point& b, const Point& p) {
    return cross(a, b, p) == 0 &&
           std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
           std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<Point> polygon(static_cast<std::size_t>(n));
    for (Point& point : polygon) {
        std::cin >> point.x >> point.y;
    }

    for (int query = 0; query < m; ++query) {
        Point p{};
        std::cin >> p.x >> p.y;

        bool inside = false;
        bool boundary = false;
        Point a = polygon.back();

        // Duyệt từng cạnh a->b của đa giác.
        for (const Point& b : polygon) {
            // Ưu tiên kiểm tra BOUNDARY: điểm nằm trên cạnh thì dừng ngay.
            if (lies_on_segment(a, b, p)) {
                boundary = true;
                break;
            }

            // Ray casting theo hướng +x. Half-open trick: khoảng nửa mở theo y
            // giúp mỗi đỉnh chung chỉ được đếm đúng một lần.
            if ((a.y > p.y) != (b.y > p.y)) {
                // Nhân chéo thay cho phép chia để giữ số học nguyên chính xác;
                // xét dấu dy để so px với hoành độ giao điểm.
                const long long dy = b.y - a.y;
                const long long left = (p.x - a.x) * dy;
                const long long right = (b.x - a.x) * (p.y - a.y);
                if ((dy > 0 && left < right) ||
                    (dy < 0 && left > right)) {
                    inside = !inside;
                }
            }
            a = b;
        }

        // Số lần cắt lẻ -> INSIDE, chẵn -> OUTSIDE.
        if (boundary) {
            std::cout << "BOUNDARY\n";
        } else if (inside) {
            std::cout << "INSIDE\n";
        } else {
            std::cout << "OUTSIDE\n";
        }
    }
    return 0;
}
