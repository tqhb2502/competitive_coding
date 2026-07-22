#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr int64_t MOD = 1'000'000'007;

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

    vector<Point> points(trap_count);
    for (Point& point : points) {
        cin >> point.y >> point.x;
    }
    sort(points.begin(), points.end());
    points.push_back({grid_size, grid_size});

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

    const auto choose = [&](const int n, const int k) -> int64_t {
        if (k < 0 || k > n) {
            return 0;
        }
        int64_t result = factorial[n] * inverse_factorial[k] % MOD;
        return result * inverse_factorial[n - k] % MOD;
    };

    vector<int64_t> ways(points.size(), 0);
    for (int current = 0; current < static_cast<int>(points.size()); ++current) {
        const Point destination = points[current];
        ways[current] = choose(destination.y + destination.x - 2,
                               destination.y - 1);

        for (int previous = 0; previous < current; ++previous) {
            const Point source = points[previous];
            if (source.x > destination.x) {
                continue;
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

    cout << ways.back() << '\n';
}
