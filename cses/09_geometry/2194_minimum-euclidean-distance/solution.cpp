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

struct Cell {
    long long x;
    long long y;

    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
};

std::uint64_t splitmix64(std::uint64_t value) {
    value += 0x9e3779b97f4a7c15ULL;
    value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31U);
}

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

long long squared_distance(const Point& a, const Point& b) {
    const long long dx = a.x - b.x;
    const long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

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

    const std::uint64_t seed = static_cast<std::uint64_t>(
        std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937_64 generator(seed);
    std::shuffle(points.begin(), points.end(), generator);

    long long best = squared_distance(points[0], points[1]);
    long long cell_size = integer_square_root(best);

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

    for (int i = 2; i < n; ++i) {
        const Point& point = points[static_cast<std::size_t>(i)];
        const long long cell_x = floor_divide(point.x, cell_size);
        const long long cell_y = floor_divide(point.y, cell_size);
        bool improved = false;

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
            cell_size = integer_square_root(best);
            grid = build_grid(i);
        } else {
            grid[Cell{cell_x, cell_y}].push_back(point);
        }
    }

    std::cout << best << '\n';
    return 0;
}
