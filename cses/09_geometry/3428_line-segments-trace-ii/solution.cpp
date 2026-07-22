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

struct SegmentLine {
    Line line;
    int left;
    int right;
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
    std::vector<SegmentLine> segments;
    segments.reserve(static_cast<std::size_t>(segment_count));
    for (int index = 0; index < segment_count; ++index) {
        int left_x, right_x;
        long long left_y, right_y;
        std::cin >> left_x >> left_y >> right_x >> right_y;
        const long long slope =
            (right_y - left_y) / (right_x - left_x);
        segments.push_back({
            {slope, left_y - slope * left_x},
            left_x,
            right_x
        });
    }
    std::sort(segments.begin(), segments.end(), [](const SegmentLine& first,
                                                     const SegmentLine& second) {
        return first.line.slope < second.line.slope;
    });

    const int point_count = maximum_x + 1;
    int tree_size = 1;
    while (tree_size < point_count) {
        tree_size *= 2;
    }
    std::vector<std::vector<Line>> tree(
        static_cast<std::size_t>(2 * tree_size)
    );
    for (const SegmentLine& segment : segments) {
        int left = segment.left + tree_size;
        int right = segment.right + tree_size;
        while (left <= right) {
            if ((left & 1) != 0) {
                tree[static_cast<std::size_t>(left)].push_back(segment.line);
                ++left;
            }
            if ((right & 1) == 0) {
                tree[static_cast<std::size_t>(right)].push_back(segment.line);
                --right;
            }
            left /= 2;
            right /= 2;
        }
    }

    std::vector<int> range_left(static_cast<std::size_t>(2 * tree_size));
    std::vector<int> range_right(static_cast<std::size_t>(2 * tree_size));
    for (int node = tree_size; node < 2 * tree_size; ++node) {
        const int position = node - tree_size;
        if (position < point_count) {
            range_left[static_cast<std::size_t>(node)] = position;
            range_right[static_cast<std::size_t>(node)] = position;
        } else {
            range_left[static_cast<std::size_t>(node)] = point_count;
            range_right[static_cast<std::size_t>(node)] = -1;
        }
    }
    for (int node = tree_size - 1; node >= 1; --node) {
        range_left[static_cast<std::size_t>(node)] =
            range_left[static_cast<std::size_t>(2 * node)];
        range_right[static_cast<std::size_t>(node)] =
            range_right[static_cast<std::size_t>(2 * node + 1)];
    }

    std::vector<long long> answer(static_cast<std::size_t>(point_count), -1);
    for (int node = 1; node < 2 * tree_size; ++node) {
        const std::vector<Line>& node_lines =
            tree[static_cast<std::size_t>(node)];
        if (node_lines.empty()) {
            continue;
        }

        std::vector<Line> hull;
        hull.reserve(node_lines.size());
        for (const Line& line : node_lines) {
            if (!hull.empty() && hull.back().slope == line.slope) {
                if (hull.back().intercept >= line.intercept) {
                    continue;
                }
                hull.pop_back();
            }
            while (hull.size() >= 2 &&
                   redundant(hull[hull.size() - 2], hull.back(), line)) {
                hull.pop_back();
            }
            hull.push_back(line);
        }

        std::size_t best = 0;
        const int left = range_left[static_cast<std::size_t>(node)];
        const int right = range_right[static_cast<std::size_t>(node)];
        for (int x = left; x <= right; ++x) {
            while (best + 1 < hull.size() &&
                   hull[best + 1].value(x) >= hull[best].value(x)) {
                ++best;
            }
            answer[static_cast<std::size_t>(x)] = std::max(
                answer[static_cast<std::size_t>(x)], hull[best].value(x)
            );
        }
    }

    for (int x = 0; x <= maximum_x; ++x) {
        if (x > 0) {
            std::cout << ' ';
        }
        std::cout << answer[static_cast<std::size_t>(x)];
    }
    std::cout << '\n';
    return 0;
}
