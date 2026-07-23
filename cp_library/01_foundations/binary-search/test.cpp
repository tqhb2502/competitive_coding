#include <cassert>
#include <cstdint>
#include <limits>
#include <random>

#include "algorithm.hpp"

namespace {

template <typename Integer>
void checkThreshold(Integer low, Integer high, Integer threshold) {
    std::uint64_t predicate_calls = 0;
    const auto predicate = [&](Integer value) {
        ++predicate_calls;
        return value >= threshold;
    };

    const auto boundary =
        cp_library::binarySearchBoundary(low, high, predicate);
    assert(boundary.last_false == static_cast<Integer>(threshold - Integer{1}));
    assert(boundary.first_true == threshold);
    assert(predicate_calls <= 8U * sizeof(Integer) + 1U);

    assert(cp_library::binarySearchFirstTrue(low, high, predicate) == threshold);
    assert(cp_library::binarySearchLastFalse(low, high, predicate) ==
           static_cast<Integer>(threshold - Integer{1}));
}

void testAdjacentSentinels() {
    int calls = 0;
    const auto boundary = cp_library::binarySearchBoundary(
        4, 5, [&](int value) {
            ++calls;
            return value >= 5;
        });
    assert(boundary.last_false == 4);
    assert(boundary.first_true == 5);
    assert(calls == 0);
}

void testExhaustivelyOnSmallDomains() {
    for (int low = -25; low <= 24; ++low) {
        for (int high = low + 1; high <= 25; ++high) {
            for (int threshold = low + 1; threshold <= high; ++threshold) {
                checkThreshold(low, high, threshold);
            }
        }
    }
}

void testExtremeDomains() {
    using Signed = std::int64_t;
    constexpr Signed signed_min = std::numeric_limits<Signed>::min();
    constexpr Signed signed_max = std::numeric_limits<Signed>::max();

    checkThreshold(signed_min, signed_max, static_cast<Signed>(signed_min + 1));
    checkThreshold(signed_min, signed_max, Signed{-1});
    checkThreshold(signed_min, signed_max, Signed{0});
    checkThreshold(signed_min, signed_max, Signed{1});
    checkThreshold(signed_min, signed_max, signed_max);

    using Unsigned = std::uint64_t;
    constexpr Unsigned unsigned_max = std::numeric_limits<Unsigned>::max();
    checkThreshold(Unsigned{0}, unsigned_max, Unsigned{1});
    checkThreshold(Unsigned{0}, unsigned_max, unsigned_max / Unsigned{2});
    checkThreshold(Unsigned{0}, unsigned_max, unsigned_max);
}

void testRandomDifferential() {
    std::mt19937 rng(0xB1A2C3U);
    std::uniform_int_distribution<int> low_distribution(-1'000'000, 999'999);
    std::uniform_int_distribution<int> width_distribution(1, 200);

    for (int iteration = 0; iteration < 20'000; ++iteration) {
        const int low = low_distribution(rng);
        const int high = low + width_distribution(rng);
        std::uniform_int_distribution<int> threshold_distribution(low + 1, high);
        const int threshold = threshold_distribution(rng);

        const auto predicate = [threshold](int value) {
            return value >= threshold;
        };
        const auto actual =
            cp_library::binarySearchBoundary(low, high, predicate);

        // Oracle tuyến tính không cần mô phỏng các bước của binary search.
        int expected = low + 1;
        while (!predicate(expected)) {
            ++expected;
        }
        assert(actual.first_true == expected);
        assert(actual.last_false == expected - 1);
    }
}

}  // namespace

int main() {
    testAdjacentSentinels();
    testExhaustivelyOnSmallDomains();
    testExtremeDomains();
    testRandomDifferential();
    return 0;
}
