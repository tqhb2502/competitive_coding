#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <utility>
#include <vector>

using Coordinate = std::int64_t;

struct Segment {
    Coordinate start_x;
    Coordinate start_y;
    Coordinate end_x;
    Coordinate end_y;

    [[nodiscard]] bool vertical() const {
        return start_x == end_x;
    }
};

// A segment [low, high] is stored in O(log n) canonical nodes.  A point query
// visits its root-to-leaf path, so every returned key belongs to a segment that
// covers the queried coordinate.
class SegmentStabber {
public:
    explicit SegmentStabber(int coordinate_count) : size_(1) {
        while (size_ < coordinate_count) {
            size_ *= 2;
        }
        keys_.resize(static_cast<std::size_t>(2 * size_));
    }

    void add(int low, int high, Coordinate key) {
        int left = low + size_;
        int right = high + size_ + 1;
        while (left < right) {
            if ((left & 1) != 0) {
                keys_[static_cast<std::size_t>(left)].insert(key);
                ++left;
            }
            if ((right & 1) != 0) {
                --right;
                keys_[static_cast<std::size_t>(right)].insert(key);
            }
            left /= 2;
            right /= 2;
        }
    }

    [[nodiscard]] std::optional<Coordinate> nearest(
        int point, Coordinate start, Coordinate finish) const {
        std::optional<Coordinate> result;

        for (int node = point + size_; node >= 1; node /= 2) {
            const auto& keys = keys_[static_cast<std::size_t>(node)];
            if (finish > start) {
                const auto it = keys.upper_bound(start);
                if (it != keys.end() && *it <= finish && (!result.has_value() || *it < *result)) {
                    result = *it;
                }
            } else {
                auto it = keys.lower_bound(start);
                if (it != keys.begin()) {
                    --it;
                    if (*it >= finish && (!result.has_value() || *it > *result)) {
                        result = *it;
                    }
                }
            }
        }
        return result;
    }

private:
    int size_;
    std::vector<std::set<Coordinate>> keys_;
};

using IntervalMap = std::map<Coordinate, Coordinate>;

[[nodiscard]] std::optional<Coordinate> nearest_collinear(
    const IntervalMap& intervals, Coordinate start, Coordinate finish) {
    if (finish > start) {
        const auto after_start = intervals.upper_bound(start);
        if (after_start != intervals.begin()) {
            const auto before = std::prev(after_start);
            if (before->second > start) {
                return 0;
            }
        }
        if (after_start != intervals.end() && after_start->first <= finish) {
            return after_start->first - start;
        }
    } else {
        const auto at_start = intervals.lower_bound(start);
        if (at_start != intervals.begin()) {
            const auto before = std::prev(at_start);
            if (before->second >= start) {
                return 0;
            }
            if (before->second >= finish) {
                return start - before->second;
            }
        }
    }
    return std::nullopt;
}

void add_interval(IntervalMap& intervals, Coordinate low, Coordinate high) {
    auto current = intervals.lower_bound(low);
    if (current != intervals.begin()) {
        const auto before = std::prev(current);
        if (before->second >= low) {
            current = before;
        }
    }

    while (current != intervals.end() && current->first <= high) {
        low = std::min(low, current->first);
        high = std::max(high, current->second);
        current = intervals.erase(current);
    }
    intervals.emplace(low, high);
}

int compressed_index(const std::vector<Coordinate>& coordinates, Coordinate value) {
    return static_cast<int>(
        std::lower_bound(coordinates.begin(), coordinates.end(), value) - coordinates.begin());
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int command_count = 0;
    std::cin >> command_count;

    std::vector<Segment> segments;
    segments.reserve(static_cast<std::size_t>(command_count));
    std::vector<Coordinate> xs{0};
    std::vector<Coordinate> ys{0};
    xs.reserve(static_cast<std::size_t>(command_count + 1));
    ys.reserve(static_cast<std::size_t>(command_count + 1));

    Coordinate current_x = 0;
    Coordinate current_y = 0;
    for (int i = 0; i < command_count; ++i) {
        char direction = 'U';
        Coordinate distance = 0;
        std::cin >> direction >> distance;

        Coordinate next_x = current_x;
        Coordinate next_y = current_y;
        if (direction == 'U') {
            next_y += distance;
        } else if (direction == 'D') {
            next_y -= distance;
        } else if (direction == 'R') {
            next_x += distance;
        } else {
            next_x -= distance;
        }

        segments.push_back({current_x, current_y, next_x, next_y});
        xs.push_back(next_x);
        ys.push_back(next_y);
        current_x = next_x;
        current_y = next_y;
    }

    std::sort(xs.begin(), xs.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    std::sort(ys.begin(), ys.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

    // Horizontal segments are indexed by their x-range and store their y;
    // vertical segments are indexed by their y-range and store their x.
    SegmentStabber horizontal_segments(static_cast<int>(xs.size()));
    SegmentStabber vertical_segments(static_cast<int>(ys.size()));
    std::vector<IntervalMap> horizontal_lines(ys.size());
    std::vector<IntervalMap> vertical_lines(xs.size());

    Coordinate travelled = 0;
    for (const Segment& segment : segments) {
        const bool is_vertical = segment.vertical();
        const Coordinate start = is_vertical ? segment.start_y : segment.start_x;
        const Coordinate finish = is_vertical ? segment.end_y : segment.end_x;
        std::optional<Coordinate> best_distance;

        if (is_vertical) {
            const int x_index = compressed_index(xs, segment.start_x);
            const auto crossing = horizontal_segments.nearest(x_index, start, finish);
            if (crossing.has_value()) {
                best_distance = std::abs(*crossing - start);
            }

            const auto overlap = nearest_collinear(
                vertical_lines[static_cast<std::size_t>(x_index)], start, finish);
            if (overlap.has_value() && (!best_distance.has_value() || *overlap < *best_distance)) {
                best_distance = overlap;
            }
        } else {
            const int y_index = compressed_index(ys, segment.start_y);
            const auto crossing = vertical_segments.nearest(y_index, start, finish);
            if (crossing.has_value()) {
                best_distance = std::abs(*crossing - start);
            }

            const auto overlap = nearest_collinear(
                horizontal_lines[static_cast<std::size_t>(y_index)], start, finish);
            if (overlap.has_value() && (!best_distance.has_value() || *overlap < *best_distance)) {
                best_distance = overlap;
            }
        }

        if (best_distance.has_value()) {
            std::cout << travelled + *best_distance << '\n';
            return 0;
        }

        const Coordinate low = std::min(start, finish);
        const Coordinate high = std::max(start, finish);
        if (is_vertical) {
            const int x_index = compressed_index(xs, segment.start_x);
            vertical_segments.add(compressed_index(ys, low), compressed_index(ys, high), segment.start_x);
            add_interval(vertical_lines[static_cast<std::size_t>(x_index)], low, high);
        } else {
            const int y_index = compressed_index(ys, segment.start_y);
            horizontal_segments.add(compressed_index(xs, low), compressed_index(xs, high), segment.start_y);
            add_interval(horizontal_lines[static_cast<std::size_t>(y_index)], low, high);
        }
        travelled += high - low;
    }

    std::cout << travelled << '\n';
    return 0;
}
