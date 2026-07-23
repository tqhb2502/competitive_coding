#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

#include "algorithm.hpp"

namespace {

constexpr unsigned long long kSeed = 0xC001D1A7EULL;

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << '\n';
        std::abort();
    }
}

[[noreturn]] void fail(int test_case, int query, const char* operation,
                       std::size_t expected, std::size_t actual) {
    std::cerr << "seed=" << kSeed << " case=" << test_case
              << " query=" << query << " operation=" << operation
              << " expected=" << expected << " actual=" << actual << '\n';
    std::abort();
}

}  // namespace

int main() {
    cp_library::CoordinateCompression<int> compression;
    require(compression.empty(), "default compression must be empty");
    require(compression.size() == 0, "empty compression has wrong size");
    require(compression.lowerBoundIndex(5) == 0, "empty lower bound is wrong");
    require(compression.upperBoundIndex(5) == 0, "empty upper bound is wrong");
    require(!compression.contains(5), "empty compression contains a value");
    require(compression.compress({}).empty(), "compressing empty input failed");

    compression.assign({8, -3, 8, 4, -3, 1000000000});
    require(
        compression.values() == std::vector<int>({-3, 4, 8, 1000000000}),
        "sort or duplicate removal failed"
    );
    require(compression.indexOf(-3) == 0, "minimum rank is wrong");
    require(compression.indexOf(1000000000) == 3, "maximum rank is wrong");
    require(compression.lowerBoundIndex(7) == 2, "lower bound is wrong");
    require(compression.upperBoundIndex(8) == 3, "upper bound is wrong");
    require(compression.valueAt(2) == 8, "inverse mapping is wrong");
    require(
        compression.compress({8, -3, 8, 4})
            == std::vector<std::size_t>({2, 0, 2, 1}),
        "batch compression is wrong"
    );

    const int minimum = std::numeric_limits<int>::lowest();
    const int maximum = std::numeric_limits<int>::max();
    compression.assign({maximum, minimum, 0, minimum});
    require(compression.indexOf(minimum) == 0,
            "minimum representable value has the wrong rank");
    require(compression.indexOf(maximum) == 2,
            "maximum representable value has the wrong rank");

    bool threw = false;
    try {
        (void)compression.indexOf(7);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    require(threw, "indexOf must reject an unregistered value");

    threw = false;
    try {
        (void)compression.valueAt(compression.size());
    } catch (const std::out_of_range&) {
        threw = true;
    }
    require(threw, "valueAt must reject size()");

    const cp_library::CoordinateCompression<int, std::greater<int>> descending(
        std::vector<int>{2, 5, 2, 1}
    );
    require(
        descending.values() == std::vector<int>({5, 2, 1}),
        "custom comparator order is wrong"
    );
    require(descending.lowerBoundIndex(4) == 1,
            "descending lower bound is wrong");
    require(descending.upperBoundIndex(2) == 2,
            "descending upper bound is wrong");
    require(descending.indexOf(2) == 1, "descending exact rank is wrong");

    std::mt19937_64 random(kSeed);
    for (int test_case = 0; test_case < 1000; ++test_case) {
        const std::size_t n = static_cast<std::size_t>(random() % 51);
        std::vector<int> input(n);
        for (int& value : input) {
            value = static_cast<int>(random() % 61) - 30;
        }

        compression.assign(input);
        const std::set<int> reference_set(input.begin(), input.end());
        const std::vector<int> reference(
            reference_set.begin(), reference_set.end()
        );
        require(compression.values() == reference,
                "random unique sorted values differ from std::set");

        const std::vector<std::size_t> ranks = compression.compress(input);
        for (std::size_t i = 0; i < input.size(); ++i) {
            if (compression.valueAt(ranks[i]) != input[i]) {
                fail(test_case, input[i], "round-trip", i, ranks[i]);
            }
        }

        for (int query = -35; query <= 35; ++query) {
            const std::size_t expected_lower = static_cast<std::size_t>(
                std::lower_bound(reference.begin(), reference.end(), query)
                - reference.begin()
            );
            const std::size_t expected_upper = static_cast<std::size_t>(
                std::upper_bound(reference.begin(), reference.end(), query)
                - reference.begin()
            );
            const std::size_t actual_lower =
                compression.lowerBoundIndex(query);
            const std::size_t actual_upper =
                compression.upperBoundIndex(query);
            if (actual_lower != expected_lower) {
                fail(test_case, query, "lower-bound",
                     expected_lower, actual_lower);
            }
            if (actual_upper != expected_upper) {
                fail(test_case, query, "upper-bound",
                     expected_upper, actual_upper);
            }
            const bool expected_contains =
                reference_set.find(query) != reference_set.end();
            require(compression.contains(query) == expected_contains,
                    "random contains result is wrong");
        }
    }

    constexpr std::size_t kLargeSize = 200000;
    std::vector<int> large(kLargeSize);
    for (std::size_t i = 0; i < large.size(); ++i) {
        large[i] = static_cast<int>((i * 7919U) % 10007U);
    }
    compression.assign(large);
    require(compression.size() == 10007,
            "large input has the wrong number of unique values");
    const std::vector<std::size_t> large_ranks = compression.compress(large);
    require(large_ranks.size() == large.size(), "large batch size is wrong");
    for (std::size_t i = 0; i < large.size(); ++i) {
        if (compression.valueAt(large_ranks[i]) != large[i]) {
            fail(1000, large[i], "large-round-trip", i, large_ranks[i]);
        }
    }
    return 0;
}
