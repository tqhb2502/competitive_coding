#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr int64_t MOD = 1'000'000'007;

// Luỹ thừa nhanh theo modulo, dùng để lấy nghịch đảo qua định lý Fermat nhỏ.
int64_t mod_power(int64_t base, int64_t exponent) {
    int64_t result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

// Một ô trên lưới; sắp xếp theo (y, x) tăng dần.
struct Point {
    int y;
    int x;

    bool operator<(const Point& other) const {
        if (y != other.y) {
            return y < other.y;
        }
        return x < other.x;
    }
};
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int grid_size;
    int trap_count;
    cin >> grid_size >> trap_count;

    // Đọc các ô bẫy, sắp xếp theo (y, x), rồi thêm đích (n, n) vào cuối.
    vector<Point> points(trap_count);
    for (Point& point : points) {
        cin >> point.y >> point.x;
    }
    sort(points.begin(), points.end());
    points.push_back({grid_size, grid_size});

    // Tiền xử lý giai thừa và nghịch đảo giai thừa tới 2n để tính C(n, k) trong O(1).
    const int factorial_limit = 2 * grid_size;
    vector<int64_t> factorial(factorial_limit + 1, 1);
    vector<int64_t> inverse_factorial(factorial_limit + 1, 1);
    for (int value = 1; value <= factorial_limit; ++value) {
        factorial[value] = factorial[value - 1] * value % MOD;
    }
    inverse_factorial[factorial_limit] =
        mod_power(factorial[factorial_limit], MOD - 2);
    for (int value = factorial_limit; value >= 1; --value) {
        inverse_factorial[value - 1] = inverse_factorial[value] * value % MOD;
    }

    // Hệ số nhị thức C(n, k) theo modulo.
    const auto choose = [&](const int n, const int k) -> int64_t {
        if (k < 0 || k > n) {
            return 0;
        }
        int64_t result = factorial[n] * inverse_factorial[k] % MOD;
        return result * inverse_factorial[n - k] % MOD;
    };

    // ways[i]: số đường từ (1,1) tới điểm i không đi qua bẫy nào đứng trước i.
    vector<int64_t> ways(points.size(), 0);
    for (int current = 0; current < static_cast<int>(points.size()); ++current) {
        const Point destination = points[current];
        // Khởi tạo bằng tổng số đường đơn điệu tới i khi chưa xét bẫy.
        ways[current] = choose(destination.y + destination.x - 2,
                               destination.y - 1);

        // Bù trừ: trừ đi các đường mà bẫy trước đó là bẫy đầu tiên bị chạm.
        for (int previous = 0; previous < current; ++previous) {
            const Point source = points[previous];
            if (source.x > destination.x) {
                continue;  // Điểm trước không nằm trên bất kỳ đường tới i.
            }

            const int down = destination.y - source.y;
            const int right = destination.x - source.x;
            const int64_t paths_between = choose(down + right, down);
            ways[current] -= ways[previous] * paths_between % MOD;
            if (ways[current] < 0) {
                ways[current] += MOD;
            }
        }
    }

    // Đích (n, n) là phần tử cuối; ways cuối cùng là đáp án.
    cout << ways.back() << '\n';
}
