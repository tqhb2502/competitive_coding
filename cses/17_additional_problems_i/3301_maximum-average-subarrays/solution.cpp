#include <cstddef>
#include <iostream>
#include <vector>

namespace {

struct Point {
    long long x = 0;
    long long y = 0;
};

long long cross(const Point& first, const Point& second, const Point& third) {
    return (second.x - first.x) * (third.y - first.y) -
           (second.y - first.y) * (third.x - first.x);
}

bool firstSlopeAtLeastSecond(const Point& first, const Point& second,
                             const Point& query) {
    return (query.y - first.y) * (query.x - second.x) >=
           (query.y - second.y) * (query.x - first.x);
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<Point> hull;
    hull.reserve(static_cast<std::size_t>(n) + 1U);
    hull.push_back({0, 0});

    long long prefix = 0;
    for (int i = 1; i <= n; ++i) {
        long long value = 0;
        std::cin >> value;
        prefix += value;
        const Point query{static_cast<long long>(i), prefix};

        std::size_t left = 0;
        std::size_t right = hull.size() - 1U;
        while (left < right) {
            const std::size_t middle = left + (right - left) / 2U;
            if (firstSlopeAtLeastSecond(hull[middle], hull[middle + 1U], query)) {
                right = middle;
            } else {
                left = middle + 1U;
            }
        }
        std::cout << query.x - hull[left].x << (i == n ? '\n' : ' ');

        while (hull.size() >= 2U &&
               cross(hull[hull.size() - 2U], hull.back(), query) <= 0) {
            hull.pop_back();
        }
        hull.push_back(query);
    }
    return 0;
}
