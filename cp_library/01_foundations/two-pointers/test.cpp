#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <random>
#include <utility>
#include <vector>

#include "algorithm.hpp"

namespace {

std::optional<std::pair<std::size_t, std::size_t>> brute_find(
    const std::vector<int>& values,
    long long target
) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        for (std::size_t j = i + 1; j < values.size(); ++j) {
            if (static_cast<long long>(values[i]) + values[j] == target) {
                return std::pair<std::size_t, std::size_t>{i, j};
            }
        }
    }
    return std::nullopt;
}

std::uint64_t brute_count(const std::vector<int>& values, long long limit) {
    std::uint64_t result = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        for (std::size_t j = i + 1; j < values.size(); ++j) {
            if (static_cast<long long>(values[i]) + values[j] <= limit) {
                ++result;
            }
        }
    }
    return result;
}

void check_case(const std::vector<int>& values, long long target) {
    const auto expected = brute_find(values, target);
    const auto actual =
        cp_library::find_pair_with_sum_sorted(values, target);

    assert(actual.has_value() == expected.has_value());
    if (actual.has_value()) {
        const auto [left, right] = *actual;
        assert(left < right);
        assert(right < values.size());
        assert(static_cast<long long>(values[left]) + values[right] == target);
    }

    assert(
        cp_library::count_pairs_with_sum_at_most_sorted(values, target) ==
        brute_count(values, target)
    );
}

}  // namespace

int main() {
    check_case({}, 0);
    check_case({7}, 14);
    check_case({2, 2, 2, 2}, 4);
    check_case({-8, -3, 0, 4, 11}, 3);
    check_case({INT_MIN, INT_MIN, INT_MAX, INT_MAX}, -1LL);

    const std::vector<int> large_positive = {INT_MAX, INT_MAX};
    const auto positive_pair = cp_library::find_pair_with_sum_sorted(
        large_positive,
        2LL * INT_MAX
    );
    assert(positive_pair.has_value());

    const std::vector<int> large_negative = {INT_MIN, INT_MIN};
    const auto negative_pair = cp_library::find_pair_with_sum_sorted(
        large_negative,
        2LL * INT_MIN
    );
    assert(negative_pair.has_value());

    const std::vector<long long> values_wider_than_int = {
        -2147483648LL, 2147483648LL
    };
    assert(cp_library::find_pair_with_sum_sorted(
               values_wider_than_int, 0LL
           ).has_value());
    assert(cp_library::count_pairs_with_sum_at_most_sorted(
               values_wider_than_int, 0LL
           ) == 1);

    std::mt19937 random(20260723);
    for (int iteration = 0; iteration < 4000; ++iteration) {
        const int n = static_cast<int>(random() % 25);
        std::vector<int> values(static_cast<std::size_t>(n));
        for (int& value : values) {
            value = static_cast<int>(random() % 31) - 15;
        }
        std::sort(values.begin(), values.end());

        const long long target = static_cast<int>(random() % 71) - 35;
        check_case(values, target);
    }

    const std::size_t large_size = 200000;
    const std::vector<int> many_equal(large_size, 1);
    assert(
        cp_library::count_pairs_with_sum_at_most_sorted(many_equal, 2LL) ==
        static_cast<std::uint64_t>(large_size) * (large_size - 1) / 2
    );

    return 0;
}
