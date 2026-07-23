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

constexpr unsigned long long kSeed = 0xD1FF3A55A9ULL;

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << '\n';
        std::abort();
    }
}

[[noreturn]] void fail(int test_case, int update_index, std::size_t position,
                       long long expected, long long actual) {
    std::cerr << "seed=" << kSeed << " case=" << test_case
              << " update=" << update_index << " position=" << position
              << " expected=" << expected << " actual=" << actual << '\n';
    std::abort();
}

void checkEqual(const cp_library::DifferenceArray<long long>& difference,
                const std::vector<long long>& brute, int test_case,
                int update_index) {
    const std::vector<long long> actual = difference.materialize();
    require(actual.size() == brute.size(), "materialized size is wrong");
    for (std::size_t i = 0; i < brute.size(); ++i) {
        if (actual[i] != brute[i]) {
            fail(test_case, update_index, i, brute[i], actual[i]);
        }
    }
}

}  // namespace

int main() {
    cp_library::DifferenceArray<long long> difference;
    require(difference.size() == 0, "default instance must be empty");
    require(difference.materialize().empty(), "empty array must stay empty");
    difference.add(0, 0, 123);
    require(difference.materialize().empty(), "empty update must be a no-op");

    const std::vector<long long> initial = {4, -2, -2, 9, 0};
    difference.assign(initial);
    difference.add(0, initial.size(), 3);
    difference.add(1, 4, -5);
    difference.add(2, 2, 900000000000000000LL);
    checkEqual(difference, {7, -4, -4, 7, 3}, -1, 2);
    checkEqual(difference, {7, -4, -4, 7, 3}, -1, 3);

    cp_library::DifferenceArray<long long> copy_source(initial);
    cp_library::DifferenceArray<long long> copy_target = copy_source;
    copy_source.add(0, initial.size(), 100);
    checkEqual(copy_target, initial, -3, 0);

    cp_library::DifferenceArray<long long> move_source(initial);
    move_source.add(1, 4, 5);
    cp_library::DifferenceArray<long long> move_target(
        std::move(move_source)
    );
    checkEqual(move_target, {4, 3, 3, 14, 0}, -4, 0);
    require(move_source.size() == 0 && move_source.materialize().empty(),
            "move constructor must leave an empty reusable source");

    move_source.reset(2);
    move_source.add(0, 2, 11);
    move_target = std::move(move_source);
    checkEqual(move_target, {11, 11}, -5, 0);
    require(move_source.size() == 0 && move_source.materialize().empty(),
            "move assignment must leave an empty reusable source");

    difference.reset(3);
    difference.add(2, 3, 8);
    checkEqual(difference, {0, 0, 8}, -2, 0);

    const long long low = std::numeric_limits<long long>::lowest() / 4;
    const long long high = std::numeric_limits<long long>::max() / 4;
    difference.assign({low, high});
    checkEqual(difference, {low, high}, -3, 0);

    bool threw = false;
    try {
        difference.add(2, 1, 4);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    require(threw, "left > right must throw");

    threw = false;
    try {
        difference.add(0, difference.size() + 1, 4);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    require(threw, "right beyond size must throw");

    threw = false;
    try {
        cp_library::DifferenceArray<long long> impossible(
            std::numeric_limits<std::size_t>::max()
        );
        (void)impossible;
    } catch (const std::length_error&) {
        threw = true;
    }
    require(threw, "SIZE_MAX must not wrap the sentinel allocation to zero");

    threw = false;
    const std::vector<long long> before_failed_reset =
        difference.materialize();
    try {
        difference.reset(std::numeric_limits<std::size_t>::max());
    } catch (const std::length_error&) {
        threw = true;
    }
    require(threw, "reset(SIZE_MAX) must throw");
    require(difference.materialize() == before_failed_reset,
            "a rejected reset must preserve the old state");

    std::mt19937_64 random(kSeed);
    for (int test_case = 0; test_case < 500; ++test_case) {
        const std::size_t n = static_cast<std::size_t>(random() % 31);
        std::vector<long long> brute(n);
        for (long long& value : brute) {
            value = static_cast<long long>(random() % 2001) - 1000;
        }

        difference.assign(brute);
        const int update_count = static_cast<int>(random() % 81);
        for (int update_index = 0; update_index < update_count; ++update_index) {
            std::size_t left = static_cast<std::size_t>(random() % (n + 1));
            std::size_t right = static_cast<std::size_t>(random() % (n + 1));
            if (left > right) {
                const std::size_t temporary = left;
                left = right;
                right = temporary;
            }
            const long long delta =
                static_cast<long long>(random() % 2001) - 1000;

            difference.add(left, right, delta);
            for (std::size_t i = left; i < right; ++i) {
                brute[i] += delta;
            }
            checkEqual(difference, brute, test_case, update_index);
        }
    }

    constexpr std::size_t kLargeSize = 200000;
    constexpr int kLargeUpdates = 200000;
    difference.reset(kLargeSize);
    for (int update_index = 0; update_index < kLargeUpdates; ++update_index) {
        difference.add(0, kLargeSize, 1000000000LL);
    }
    const std::vector<long long> large = difference.materialize();
    require(large.front() == 200000000000000LL,
            "large full-range updates failed at the first position");
    require(large.back() == 200000000000000LL,
            "large full-range updates failed at the last position");
    return 0;
}
