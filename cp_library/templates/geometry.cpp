#include <iostream>

using namespace std;

struct Point {
    long long x = 0;
    long long y = 0;
};

Point operator-(const Point& first, const Point& second) {
    return {first.x - second.x, first.y - second.y};
}

// Tích có hướng của hai vector; tích tọa độ cần nằm trong miền long long.
long long cross(const Point& first, const Point& second) {
    return first.x * second.y - first.y * second.x;
}

// Tích có hướng (first-origin) x (second-origin).
long long cross(const Point& origin, const Point& first, const Point& second) {
    return cross(first - origin, second - origin);
}

// Trả về 1 nếu rẽ trái, -1 nếu rẽ phải và 0 nếu ba điểm thẳng hàng.
int orientation(const Point& origin, const Point& first, const Point& second) {
    const long long area = cross(origin, first, second);
    return (area > 0) - (area < 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Bổ sung phần đọc điểm và thuật toán hình học theo yêu cầu của bài.
    return 0;
}
