#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

struct Point {
    long long x;
    long long y;
};

// Ô lưới (cell) trong spatial hash grid: chỉ số ô theo x và y.
struct Cell {
    long long x;
    long long y;

    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
};

// Trộn bit splitmix64 để phân tán khóa (tránh va chạm hash có chủ đích).
std::uint64_t splitmix64(std::uint64_t value) {
    value += 0x9e3779b97f4a7c15ULL;
    value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31U);
}

// Hàm băm cho Cell, có salt ngẫu nhiên theo thời gian để chống adversarial.
struct CellHash {
    std::size_t operator()(const Cell& cell) const {
        static const std::uint64_t salt = static_cast<std::uint64_t>(
            std::chrono::steady_clock::now().time_since_epoch().count());
        const std::uint64_t first =
            splitmix64(static_cast<std::uint64_t>(cell.x) + salt);
        const std::uint64_t second =
            splitmix64(static_cast<std::uint64_t>(cell.y) + salt);
        return static_cast<std::size_t>(
            first ^ (second + 0x9e3779b97f4a7c15ULL +
                     (first << 6U) + (first >> 2U)));
    }
};

// Bình phương khoảng cách Euclid: dùng SỐ HỌC NGUYÊN nên tuyệt đối chính xác.
long long squared_distance(const Point& a, const Point& b) {
    const long long dx = a.x - b.x;
    const long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Căn bậc hai NGUYÊN: lấy floor(sqrt(value)), tinh chỉnh để loại sai số float.
long long integer_square_root(long long value) {
    long long root = static_cast<long long>(
        std::sqrt(static_cast<long double>(value)));
    while (root + 1 <= value / (root + 1)) {
        ++root;
    }
    while (root > value / root) {
        --root;
    }
    return root;
}

// Chia làm tròn xuống (floor division) để chỉ số ô đúng cả với tọa độ âm.
long long floor_divide(long long value, long long positive_divisor) {
    long long quotient = value / positive_divisor;
    if (value % positive_divisor < 0) {
        --quotient;
    }
    return quotient;
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

    // Bước 1: xáo trộn ngẫu nhiên để phá mọi trường hợp xấu (kỳ vọng O(n)).
    const std::uint64_t seed = static_cast<std::uint64_t>(
        std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937_64 generator(seed);
    std::shuffle(points.begin(), points.end(), generator);

    // Bước 2: best = bình phương khoảng cách của hai điểm đầu tiên.
    long long best = squared_distance(points[0], points[1]);
    // Bước 3: kích thước ô lưới s = isqrt(best) (số nguyên, s >= 1).
    long long cell_size = integer_square_root(best);

    // Dựng lại toàn bộ grid từ các điểm points[0..last_index] với cell_size mới.
    using Grid = std::unordered_map<Cell, std::vector<Point>, CellHash>;
    auto build_grid = [&](int last_index) {
        Grid result;
        result.reserve(static_cast<std::size_t>(2 * (last_index + 1)));
        for (int i = 0; i <= last_index; ++i) {
            const Point& point = points[static_cast<std::size_t>(i)];
            const Cell cell{floor_divide(point.x, cell_size),
                            floor_divide(point.y, cell_size)};
            result[cell].push_back(point);
        }
        return result;
    };

    Grid grid = build_grid(1);

    // Bước 4: chèn từng điểm còn lại theo kiểu incremental.
    for (int i = 2; i < n; ++i) {
        const Point& point = points[static_cast<std::size_t>(i)];
        const long long cell_x = floor_divide(point.x, cell_size);
        const long long cell_y = floor_divide(point.y, cell_size);
        bool improved = false;

        // Chỉ cần duyệt 3x3 ô quanh ô của point (xem chứng minh trong idea.txt).
        for (long long delta_x = -1; delta_x <= 1; ++delta_x) {
            for (long long delta_y = -1; delta_y <= 1; ++delta_y) {
                const auto bucket = grid.find(
                    Cell{cell_x + delta_x, cell_y + delta_y});
                if (bucket == grid.end()) {
                    continue;
                }
                for (const Point& previous : bucket->second) {
                    const long long candidate =
                        squared_distance(point, previous);
                    if (candidate < best) {
                        best = candidate;
                        improved = true;
                    }
                }
            }
        }

        if (improved) {
            // best giảm: tính lại kích thước ô và rebuild toàn bộ grid.
            cell_size = integer_square_root(best);
            grid = build_grid(i);
        } else {
            // Không cải thiện: chỉ chèn point vào ô của nó.
            grid[Cell{cell_x, cell_y}].push_back(point);
        }
    }

    std::cout << best << '\n';
    return 0;
}
