#include <algorithm>
#include <cassert>
#include <cstddef>
#include <random>
#include <utility>
#include <vector>

#include "algorithm.hpp"

namespace {

long long range_sum(
    const std::vector<int>& values,
    std::size_t left,
    std::size_t right
) {
    long long result = 0;
    for (std::size_t index = left; index < right; ++index) {
        result += values[index];
    }
    return result;
}

void check_fixed(const std::vector<int>& values, std::size_t width) {
    long long current_sum = 0;
    std::vector<std::pair<std::size_t, std::size_t>> actual_windows;

    cp_library::for_each_fixed_window(
        values.size(),
        width,
        [&](std::size_t index) { current_sum += values[index]; },
        [&](std::size_t index) { current_sum -= values[index]; },
        [&](std::size_t left, std::size_t right) {
            actual_windows.emplace_back(left, right);
            assert(right - left == width);
            assert(current_sum == range_sum(values, left, right));
        }
    );

    std::vector<std::pair<std::size_t, std::size_t>> expected_windows;
    if (width == 0) {
        for (std::size_t position = 0; position <= values.size(); ++position) {
            expected_windows.emplace_back(position, position);
        }
        assert(current_sum == 0);
    } else if (width <= values.size()) {
        for (
            std::size_t left = 0;
            left + width <= values.size();
            ++left
        ) {
            expected_windows.emplace_back(left, left + width);
        }
        assert(
            current_sum ==
            range_sum(values, values.size() - width, values.size())
        );
    } else {
        assert(current_sum == 0);
    }

    assert(actual_windows == expected_windows);
}

void check_variable(const std::vector<int>& values, long long limit) {
    long long current_sum = 0;
    std::vector<std::pair<std::size_t, std::size_t>> actual_windows;

    cp_library::for_each_longest_valid_suffix(
        values.size(),
        [&](std::size_t index) { current_sum += values[index]; },
        [&](std::size_t index) { current_sum -= values[index]; },
        [&]() { return current_sum <= limit; },
        [&](std::size_t left, std::size_t right) {
            actual_windows.emplace_back(left, right);
            assert(current_sum == range_sum(values, left, right));
            assert(current_sum <= limit);
        }
    );

    std::vector<std::pair<std::size_t, std::size_t>> expected_windows;
    for (std::size_t right = 1; right <= values.size(); ++right) {
        std::size_t left = 0;
        while (left < right && range_sum(values, left, right) > limit) {
            ++left;
        }
        expected_windows.emplace_back(left, right);
    }

    assert(actual_windows == expected_windows);
}

}  // namespace

int main() {
    check_fixed({}, 0);
    check_fixed({}, 1);
    check_fixed({7}, 0);
    check_fixed({7}, 1);
    check_fixed({7}, 2);
    check_fixed({3, 3, 3, 3}, 2);
    check_fixed({-1000000000, 1000000000, 1000000000}, 2);

    check_variable({}, 0);
    check_variable({7}, 3);
    check_variable({7}, 7);
    check_variable({0, 0, 0}, 0);
    check_variable({2, 1, 3, 2, 4}, 5);

    std::mt19937 random(20260723);
    for (int iteration = 0; iteration < 3000; ++iteration) {
        const std::size_t n = random() % 25;
        std::vector<int> values(n);
        for (int& value : values) {
            value = static_cast<int>(random() % 11);
        }

        const std::size_t width = random() % (n + 3);
        check_fixed(values, width);

        const long long limit = random() % 31;
        check_variable(values, limit);
    }

    const std::size_t large_size = 200000;
    const std::vector<int> many_equal(large_size, 1);
    std::size_t visited = 0;
    long long sum = 0;
    cp_library::for_each_fixed_window(
        many_equal.size(),
        946,
        [&](std::size_t index) { sum += many_equal[index]; },
        [&](std::size_t index) { sum -= many_equal[index]; },
        [&](std::size_t left, std::size_t right) {
            assert(right - left == 946);
            assert(sum == 946);
            ++visited;
        }
    );
    assert(visited == large_size - 946 + 1);

    return 0;
}
