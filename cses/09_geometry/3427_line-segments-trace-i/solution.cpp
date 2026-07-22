#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using i128 = __int128_t;

struct Line {
    long long slope;
    long long intercept;

    long long value(const long long x) const {
        return slope * x + intercept;
    }
};

bool redundant(const Line& first, const Line& middle, const Line& last) {
    return static_cast<i128>(first.intercept - middle.intercept) *
               (last.slope - middle.slope) >=
           static_cast<i128>(middle.intercept - last.intercept) *
               (middle.slope - first.slope);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int segment_count, maximum_x;
    std::cin >> segment_count >> maximum_x;
    std::vector<Line> lines;
    lines.reserve(static_cast<std::size_t>(segment_count));
    for (int index = 0; index < segment_count; ++index) {
        long long left_y, right_y;
        std::cin >> left_y >> right_y;
        lines.push_back({
            (right_y - left_y) / maximum_x,
            left_y
        });
    }

    std::sort(lines.begin(), lines.end(), [](const Line& first,
                                               const Line& second) {
        if (first.slope != second.slope) {
            return first.slope < second.slope;
        }
        return first.intercept > second.intercept;
    });

    std::vector<Line> hull;
    hull.reserve(lines.size());
    for (const Line& line : lines) {
        if (!hull.empty() && hull.back().slope == line.slope) {
            continue;
        }
        while (hull.size() >= 2 &&
               redundant(hull[hull.size() - 2], hull.back(), line)) {
            hull.pop_back();
        }
        hull.push_back(line);
    }

    std::size_t best = 0;
    for (int x = 0; x <= maximum_x; ++x) {
        while (best + 1 < hull.size() &&
               hull[best + 1].value(x) >= hull[best].value(x)) {
            ++best;
        }
        if (x > 0) {
            std::cout << ' ';
        }
        std::cout << hull[best].value(x);
    }
    std::cout << '\n';
    return 0;
}
