#include <cstddef>
#include <iostream>
#include <vector>

namespace {

// Điểm prefix (hoành độ là chỉ số, tung độ là tổng tiền tố).
struct Point {
    long long x = 0;
    long long y = 0;
};

// Tích có hướng: > 0 nếu ba điểm tạo một lần rẽ trái (bao lồi dưới).
long long cross(const Point& first, const Point& second, const Point& third) {
    return (second.x - first.x) * (third.y - first.y) -
           (second.y - first.y) * (third.x - first.x);
}

// So sánh hệ số góc tới điểm query bằng phép nhân chéo số nguyên:
// trả về true nếu slope(first->query) >= slope(second->query).
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

    // Bao lồi dưới của các điểm prefix, khởi tạo với điểm (0, 0) = P_0.
    std::vector<Point> hull;
    hull.reserve(static_cast<std::size_t>(n) + 1U);
    hull.push_back({0, 0});

    long long prefix = 0;
    for (int i = 1; i <= n; ++i) {
        long long value = 0;
        std::cin >> value;
        prefix += value;
        // Điểm truy vấn ứng với đoạn kết thúc tại i.
        const Point query{static_cast<long long>(i), prefix};

        // Tìm kiếm nhị phân đỉnh bao cho hệ số góc lớn nhất tới query.
        // Hệ số góc tới query tăng rồi giảm; khi hòa đi sang trái để lấy
        // chỉ số j nhỏ nhất, tức đoạn con dài nhất.
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
        // Độ dài đoạn tối ưu là i - j.
        std::cout << query.x - hull[left].x << (i == n ? '\n' : ' ');

        // Thêm query vào bao lồi dưới: xóa các đỉnh không còn tạo rẽ trái.
        while (hull.size() >= 2U &&
               cross(hull[hull.size() - 2U], hull.back(), query) <= 0) {
            hull.pop_back();
        }
        hull.push_back(query);
    }
    return 0;
}
