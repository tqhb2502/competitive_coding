#include <algorithm>
#include <iostream>
#include <vector>

struct Point {
    long long x;
    long long y;

    // Sắp xếp các điểm theo thứ tự (x, y) tăng dần.
    bool operator<(const Point& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
};

// Cross product nguyên (exact integer): dương -> quẹo trái, âm -> quẹo phải,
// bằng 0 -> ba điểm thẳng hàng (collinear). Dùng long long nên không tràn số.
long long cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) -
           (b.y - a.y) * (c.x - a.x);
}

// Dựng một nửa hull theo Andrew's monotone chain. Chỉ pop khi cross < 0 (góc
// quẹo phải THỰC SỰ) để GIỮ LẠI các điểm collinear nằm trên cạnh biên.
std::vector<Point> build_half(const std::vector<Point>& points) {
    std::vector<Point> half;
    half.reserve(points.size());
    for (const Point& point : points) {
        while (half.size() >= 2 &&
               cross(half[half.size() - 2], half.back(), point) < 0) {
            half.pop_back();
        }
        half.push_back(point);
    }
    return half;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<Point> points(static_cast<std::size_t>(n));
    for (Point& point : points) {
        std::cin >> point.x >> point.y;
    }
    std::sort(points.begin(), points.end());

    // CSES bảo đảm diện tích hull dương. Xử lý thêm trường hợp suy biến (tất cả
    // điểm thẳng hàng) để tránh hai monotone chain in trùng các điểm collinear
    // nếu điều kiện đó về sau bị nới lỏng.
    bool all_collinear = true;
    for (std::size_t i = 2; i < points.size(); ++i) {
        if (cross(points[0], points[1], points[i]) != 0) {
            all_collinear = false;
            break;
        }
    }
    if (all_collinear) {
        std::cout << points.size() << '\n';
        for (const Point& point : points) {
            std::cout << point.x << ' ' << point.y << '\n';
        }
        return 0;
    }

    // Lower hull duyệt trái -> phải; upper hull duyệt phải -> trái (đảo mảng).
    std::vector<Point> lower = build_half(points);
    std::reverse(points.begin(), points.end());
    std::vector<Point> upper = build_half(points);

    // Bỏ điểm cuối của mỗi chain (trùng với đỉnh đầu của chain kia) rồi ghép lại.
    std::vector<Point> hull;
    hull.reserve(lower.size() + upper.size() - 2);
    hull.insert(hull.end(), lower.begin(), lower.end() - 1);
    hull.insert(hull.end(), upper.begin(), upper.end() - 1);

    std::cout << hull.size() << '\n';
    for (const Point& point : hull) {
        std::cout << point.x << ' ' << point.y << '\n';
    }
    return 0;
}
