#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using i64 = std::int64_t;
using i128 = __int128_t;

struct Point {
    i64 x;
    i64 y;
};

// In một số __int128 (kiểu này không được cout hỗ trợ trực tiếp).
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

    // Đọc số đỉnh và toàn bộ tọa độ theo thứ tự vòng quanh đa giác.
    int vertex_count = 0;
    std::cin >> vertex_count;
    std::vector<Point> polygon(static_cast<std::size_t>(vertex_count));
    for (Point& point : polygon) {
        std::cin >> point.x >> point.y;
    }

    // Công thức shoelace: cộng dồn cross product của các cạnh liên tiếp.
    // Dùng __int128 để tránh tràn số vì tổng có thể cỡ ~1e21.
    i128 doubled_area = 0;
    for (int index = 0; index < vertex_count; ++index) {
        const Point& current = polygon[static_cast<std::size_t>(index)];
        const Point& next = polygon[static_cast<std::size_t>((index + 1) % vertex_count)];
        doubled_area += static_cast<i128>(current.x) * next.y -
                        static_cast<i128>(next.x) * current.y;
    }

    // Lấy trị tuyệt đối để ra 2a đúng bất kể chiều duyệt (clockwise hay ngược lại).
    if (doubled_area < 0) {
        doubled_area = -doubled_area;
    }
    print_integer(doubled_area);
    std::cout << '\n';
}
