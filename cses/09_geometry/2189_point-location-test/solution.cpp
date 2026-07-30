#include <cstdint>
#include <iostream>

using i64 = std::int64_t;
using i128 = __int128_t;

struct Point {
    i64 x;
    i64 y;
};

// cross product (tích có hướng): trả về z-component của (p2 - p1) x (p3 - p1).
// Dùng __int128 để giữ số học chính xác, tránh tràn số khi tọa độ lớn.
[[nodiscard]] i128 cross(const Point& first, const Point& second, const Point& third) {
    return static_cast<i128>(second.x - first.x) * (third.y - first.y) -
           static_cast<i128>(second.y - first.y) * (third.x - first.x);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int test_count = 0;
    std::cin >> test_count;

    // Xử lý từng truy vấn độc lập.
    while (test_count-- > 0) {
        Point first{};
        Point second{};
        Point third{};
        std::cin >> first.x >> first.y >> second.x >> second.y >> third.x >> third.y;

        // Dấu của cross cho biết vị trí của p3 so với hướng nhìn từ p1 sang p2.
        const i128 direction = cross(first, second, third);
        if (direction > 0) {
            std::cout << "LEFT\n";   // quay ngược chiều kim đồng hồ -> bên trái
        } else if (direction < 0) {
            std::cout << "RIGHT\n";  // quay theo chiều kim đồng hồ -> bên phải
        } else {
            std::cout << "TOUCH\n";  // thẳng hàng -> nằm trên đường thẳng
        }
    }
}
