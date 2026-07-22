#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using i64 = std::int64_t;
using i128 = __int128_t;

struct Point {
    i64 x;
    i64 y;
};

void print_integer(i128 value) {
    if (value == 0) {
        std::cout << '0';
        return;
    }
    if (value < 0) {
        std::cout << '-';
        value = -value;
    }
    std::string digits;
    while (value > 0) {
        digits.push_back(static_cast<char>('0' + static_cast<int>(value % 10)));
        value /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    std::cout << digits;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertex_count = 0;
    std::cin >> vertex_count;
    std::vector<Point> polygon(static_cast<std::size_t>(vertex_count));
    for (Point& point : polygon) {
        std::cin >> point.x >> point.y;
    }

    i128 doubled_area = 0;
    i128 boundary_points = 0;
    for (int index = 0; index < vertex_count; ++index) {
        const Point& current = polygon[static_cast<std::size_t>(index)];
        const Point& next = polygon[static_cast<std::size_t>((index + 1) % vertex_count)];
        doubled_area += static_cast<i128>(current.x) * next.y -
                        static_cast<i128>(next.x) * current.y;
        boundary_points += std::gcd(next.x - current.x, next.y - current.y);
    }
    if (doubled_area < 0) {
        doubled_area = -doubled_area;
    }

    const i128 interior_points = (doubled_area - boundary_points + 2) / 2;
    print_integer(interior_points);
    std::cout << ' ';
    print_integer(boundary_points);
    std::cout << '\n';
}
