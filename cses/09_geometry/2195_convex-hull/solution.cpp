#include <algorithm>
#include <iostream>
#include <vector>

struct Point {
    long long x;
    long long y;

    bool operator<(const Point& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
};

long long cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) -
           (b.y - a.y) * (c.x - a.x);
}

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

    // CSES guarantees positive hull area. Handling the degenerate case as well
    // prevents the two monotone chains from printing interior collinear points
    // twice if that precondition is ever relaxed.
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

    std::vector<Point> lower = build_half(points);
    std::reverse(points.begin(), points.end());
    std::vector<Point> upper = build_half(points);

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
