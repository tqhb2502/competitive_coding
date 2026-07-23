#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <functional>
#include <random>
#include <vector>

#include "algorithm.hpp"

namespace {

struct AbsoluteValueLess {
    bool operator()(int left, int right) const {
        const long long left_magnitude =
            left < 0 ? -static_cast<long long>(left) : left;
        const long long right_magnitude =
            right < 0 ? -static_cast<long long>(right) : right;
        return left_magnitude < right_magnitude;
    }
};

template <class Better>
std::vector<std::size_t> brute_indices(
    const std::vector<int>& values,
    std::size_t width,
    Better better
) {
    std::vector<std::size_t> result;
    if (width == 0 || width > values.size()) {
        return result;
    }

    for (
        std::size_t left = 0;
        left + width <= values.size();
        ++left
    ) {
        std::size_t best = left;
        for (std::size_t index = left + 1; index < left + width; ++index) {
            if (
                better(values[index], values[best]) ||
                (
                    !better(values[best], values[index]) &&
                    !better(values[index], values[best])
                )
            ) {
                best = index;
            }
        }
        result.push_back(best);
    }
    return result;
}

std::vector<int> values_at(
    const std::vector<int>& values,
    const std::vector<std::size_t>& indices
) {
    std::vector<int> result;
    for (const std::size_t index : indices) {
        result.push_back(values[index]);
    }
    return result;
}

void check_case(const std::vector<int>& values, std::size_t width) {
    const auto expected_min =
        brute_indices(values, width, std::less<int>{});
    const auto expected_max =
        brute_indices(values, width, std::greater<int>{});

    assert(
        cp_library::sliding_window_min_indices(values, width) ==
        expected_min
    );
    assert(
        cp_library::sliding_window_max_indices(values, width) ==
        expected_max
    );
    assert(
        cp_library::sliding_window_min_values(values, width) ==
        values_at(values, expected_min)
    );
    assert(
        cp_library::sliding_window_max_values(values, width) ==
        values_at(values, expected_max)
    );

    const auto by_absolute_value =
        cp_library::sliding_window_extreme_indices(
            values,
            width,
            AbsoluteValueLess{}
        );
    const auto expected_absolute = brute_indices(
        values,
        width,
        AbsoluteValueLess{}
    );
    assert(by_absolute_value == expected_absolute);
}

}  // namespace

int main() {
    check_case({}, 0);
    check_case({}, 1);
    check_case({7}, 0);
    check_case({7}, 1);
    check_case({7}, 2);
    check_case({2, 2, 2, 2}, 1);
    check_case({2, 2, 2, 2}, 3);
    check_case({INT_MIN, 0, INT_MAX, INT_MIN, INT_MAX}, 2);
    check_case({5, 4, 3, 2, 1}, 5);
    check_case({1, 2, 3, 4, 5}, 2);

    const std::vector<int> duplicates = {4, 2, 2, 5};
    const std::vector<std::size_t> expected_rightmost_min = {2, 2};
    assert(
        cp_library::sliding_window_min_indices(duplicates, 3) ==
        expected_rightmost_min
    );

    std::mt19937 random(20260723);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const std::size_t n = random() % 30;
        std::vector<int> values(n);
        for (int& value : values) {
            value = static_cast<int>(random() % 15) - 7;
        }

        const std::size_t width = random() % (n + 3);
        check_case(values, width);
    }

    const std::size_t large_size = 200000;
    const std::size_t large_width = 946;
    const std::vector<int> many_equal(large_size, 7);
    const auto large_indices =
        cp_library::sliding_window_min_indices(many_equal, large_width);
    assert(large_indices.size() == large_size - large_width + 1);
    for (std::size_t window = 0; window < large_indices.size(); ++window) {
        assert(large_indices[window] == window + large_width - 1);
    }

    return 0;
}
