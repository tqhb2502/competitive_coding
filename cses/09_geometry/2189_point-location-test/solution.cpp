#include <cstdint>
#include <iostream>

using i64 = std::int64_t;
using i128 = __int128_t;

struct Point {
    i64 x;
    i64 y;
};

[[nodiscard]] i128 cross(const Point& first, const Point& second, const Point& third) {
    return static_cast<i128>(second.x - first.x) * (third.y - first.y) -
           static_cast<i128>(second.y - first.y) * (third.x - first.x);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int test_count = 0;
    std::cin >> test_count;

    while (test_count-- > 0) {
        Point first{};
        Point second{};
        Point third{};
        std::cin >> first.x >> first.y >> second.x >> second.y >> third.x >> third.y;

        const i128 direction = cross(first, second, third);
        if (direction > 0) {
            std::cout << "LEFT\n";
        } else if (direction < 0) {
            std::cout << "RIGHT\n";
        } else {
            std::cout << "TOUCH\n";
        }
    }
}
