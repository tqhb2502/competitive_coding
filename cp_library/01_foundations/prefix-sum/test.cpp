#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

#include "algorithm.hpp"

namespace {

constexpr unsigned long long kSeed = 0xC0FFEE1234ULL;

[[noreturn]] void fail(int test_case, std::size_t left, std::size_t right,
                       long long expected, long long actual) {
    std::cerr << "seed=" << kSeed << " case=" << test_case
              << " range=[" << left << ", " << right << ")"
              << " expected=" << expected << " actual=" << actual << '\n';
    std::abort();
}

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << '\n';
        std::abort();
    }
}

}  // namespace

int main() {
    cp_library::PrefixSum<long long> sums;
    require(sums.size() == 0, "default instance must be empty");
    require(sums.prefix(0) == 0, "empty prefix must be zero");
    require(sums.query(0, 0) == 0, "empty range must have sum zero");

    const std::vector<int> values = {5, -3, 5, 0, 8};
    sums.assign(values);
    require(sums.size() == values.size(), "assign must replace the old array");
    require(sums.prefix(3) == 7, "wrong prefix sum");
    require(sums.query(1, 4) == 2, "wrong typical range sum");
    require(sums.query(2, 2) == 0, "empty subrange must have sum zero");
    require(sums.query(0, values.size()) == 15, "wrong full range sum");

    cp_library::PrefixSum<long long> copy_source(values);
    cp_library::PrefixSum<long long> copy_target = copy_source;
    copy_source.assign(std::vector<int>{1});
    require(copy_target.query(0, values.size()) == 15,
            "copy must own an independent prefix array");

    cp_library::PrefixSum<long long> move_source(values);
    cp_library::PrefixSum<long long> move_target(std::move(move_source));
    require(move_target.query(0, values.size()) == 15,
            "move constructor lost the prefix sums");
    require(move_source.size() == 0 && move_source.query(0, 0) == 0,
            "move constructor must leave an empty reusable source");

    move_source.assign(std::vector<int>{9, -4});
    move_target = std::move(move_source);
    require(move_target.query(0, 2) == 5,
            "move assignment lost the prefix sums");
    require(move_source.size() == 0 && move_source.prefix(0) == 0,
            "move assignment must leave an empty reusable source");

    const std::vector<int> large(200000, 1000000000);
    sums.assign(large);
    require(sums.query(0, large.size()) == 200000000000000LL,
            "accumulator must handle sums larger than 32 bits");

    bool threw = false;
    try {
        (void)sums.query(2, 1);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    require(threw, "left > right must throw");

    threw = false;
    try {
        (void)sums.prefix(large.size() + 1);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    require(threw, "an endpoint beyond size must throw");

    std::mt19937_64 random(kSeed);
    for (int test_case = 0; test_case < 600; ++test_case) {
        const std::size_t n = static_cast<std::size_t>(random() % 41);
        std::vector<long long> random_values(n);
        for (long long& value : random_values) {
            value = static_cast<long long>(random() % 2001) - 1000;
        }

        sums.assign(random_values);
        for (std::size_t left = 0; left <= n; ++left) {
            long long brute_sum = 0;
            for (std::size_t right = left; right <= n; ++right) {
                if (right > left) {
                    brute_sum += random_values[right - 1];
                }
                const long long actual = sums.query(left, right);
                if (actual != brute_sum) {
                    fail(test_case, left, right, brute_sum, actual);
                }
            }
        }
    }

    const std::vector<long long> one = {
        std::numeric_limits<long long>::lowest() + 1
    };
    sums.assign(one);
    require(sums.query(0, 1) == one[0], "single boundary value was changed");
    return 0;
}
