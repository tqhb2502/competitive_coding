#include <algorithm>
#include <cstdint>
#include <iostream>

using i64 = std::int64_t;
using i128 = __int128_t;

struct Point {
    i64 x;
    i64 y;
};

// Cross product (b - a) x (c - a): dấu cho biết orientation của điểm c so với
// hướng a -> b (trái > 0, phải < 0, thẳng hàng = 0). Dùng __int128 tránh tràn số.
[[nodiscard]] i128 cross(const Point& first, const Point& second, const Point& third) {
    return static_cast<i128>(second.x - first.x) * (third.y - first.y) -
           static_cast<i128>(second.y - first.y) * (third.x - first.x);
}

// Kiểm tra on-segment bằng bounding box: điểm (đã thẳng hàng) có nằm trong hình
// chữ nhật bao của đoạn first-second theo cả hai trục x và y hay không.
[[nodiscard]] bool on_segment(const Point& first, const Point& second, const Point& point) {
    return std::min(first.x, second.x) <= point.x && point.x <= std::max(first.x, second.x) &&
           std::min(first.y, second.y) <= point.y && point.y <= std::max(first.y, second.y);
}

// Hai giá trị trái dấu (đều khác 0): dùng để xét hai đầu mút nằm khác phía.
[[nodiscard]] bool opposite_signs(i128 first, i128 second) {
    return (first < 0 && second > 0) || (first > 0 && second < 0);
}

// Xác định hai đoạn first-second và third-fourth có giao nhau hay không.
[[nodiscard]] bool intersect(const Point& first, const Point& second,
                             const Point& third, const Point& fourth) {
    // Bốn orientation: vị trí của third, fourth so với đường thẳng first-second
    // và vị trí của first, second so với đường thẳng third-fourth.
    const i128 third_side = cross(first, second, third);
    const i128 fourth_side = cross(first, second, fourth);
    const i128 first_side = cross(third, fourth, first);
    const i128 second_side = cross(third, fourth, second);

    // Giao nhau "thật sự" (proper): mỗi đoạn có hai đầu mút nằm khác phía so với
    // đường thẳng chứa đoạn kia -> cắt nhau tại đúng một điểm bên trong.
    if (opposite_signs(third_side, fourth_side) && opposite_signs(first_side, second_side)) {
        return true;
    }
    // Trường hợp biên (collinear / chạm đầu mút): một điểm thẳng hàng với đoạn kia
    // (orientation = 0) và nằm trong bounding box của đoạn đó.
    if (third_side == 0 && on_segment(first, second, third)) {
        return true;
    }
    if (fourth_side == 0 && on_segment(first, second, fourth)) {
        return true;
    }
    if (first_side == 0 && on_segment(third, fourth, first)) {
        return true;
    }
    return second_side == 0 && on_segment(third, fourth, second);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc số truy vấn rồi xử lý từng truy vấn trong O(1).
    int test_count = 0;
    std::cin >> test_count;
    while (test_count-- > 0) {
        Point first{};
        Point second{};
        Point third{};
        Point fourth{};
        std::cin >> first.x >> first.y >> second.x >> second.y >>
            third.x >> third.y >> fourth.x >> fourth.y;
        std::cout << (intersect(first, second, third, fourth) ? "YES\n" : "NO\n");
    }
}
