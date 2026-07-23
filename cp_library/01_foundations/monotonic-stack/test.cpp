#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <random>
#include <vector>

#include "algorithm.hpp"

namespace {

enum class Relation {
    Less,
    Greater
};

bool qualifies(int candidate,
               int current,
               Relation relation,
               cp_library::MonotonicStrictness strictness) {
    if (relation == Relation::Less) {
        if (strictness == cp_library::MonotonicStrictness::Strict) {
            return candidate < current;
        }
        return candidate <= current;
    }
    if (strictness == cp_library::MonotonicStrictness::Strict) {
        return candidate > current;
    }
    return candidate >= current;
}

std::vector<std::size_t> brutePrevious(
    const std::vector<int>& values,
    Relation relation,
    cp_library::MonotonicStrictness strictness) {
    const std::size_t size = values.size();
    std::vector<std::size_t> answer(size, size);
    for (std::size_t index = 0; index < size; ++index) {
        for (std::size_t candidate = index; candidate > 0;) {
            --candidate;
            if (qualifies(values[candidate], values[index], relation,
                          strictness)) {
                answer[index] = candidate;
                break;
            }
        }
    }
    return answer;
}

std::vector<std::size_t> bruteNext(
    const std::vector<int>& values,
    Relation relation,
    cp_library::MonotonicStrictness strictness) {
    const std::size_t size = values.size();
    std::vector<std::size_t> answer(size, size);
    for (std::size_t index = 0; index < size; ++index) {
        for (std::size_t candidate = index + 1; candidate < size; ++candidate) {
            if (qualifies(values[candidate], values[index], relation,
                          strictness)) {
                answer[index] = candidate;
                break;
            }
        }
    }
    return answer;
}

void checkAgainstBrute(const std::vector<int>& values) {
    const cp_library::MonotonicStrictness modes[] = {
        cp_library::MonotonicStrictness::Strict,
        cp_library::MonotonicStrictness::NonStrict};

    for (const auto mode : modes) {
        assert(cp_library::nearestPreviousLess(values, mode) ==
               brutePrevious(values, Relation::Less, mode));
        assert(cp_library::nearestNextLess(values, mode) ==
               bruteNext(values, Relation::Less, mode));
        assert(cp_library::nearestPreviousGreater(values, mode) ==
               brutePrevious(values, Relation::Greater, mode));
        assert(cp_library::nearestNextGreater(values, mode) ==
               bruteNext(values, Relation::Greater, mode));
    }
}

void testExplicitDuplicates() {
    const std::vector<int> values = {2, 3, 3, 1};
    const std::size_t none = values.size();

    assert(cp_library::nearestPreviousLess(
               values, cp_library::MonotonicStrictness::Strict) ==
           std::vector<std::size_t>({none, 0, 0, none}));
    assert(cp_library::nearestPreviousLess(
               values, cp_library::MonotonicStrictness::NonStrict) ==
           std::vector<std::size_t>({none, 0, 1, none}));

    const std::vector<int> equal = {5, 5, 5};
    assert(cp_library::nearestNextGreater(
               equal, cp_library::MonotonicStrictness::Strict) ==
           std::vector<std::size_t>({3, 3, 3}));
    assert(cp_library::nearestNextGreater(
               equal, cp_library::MonotonicStrictness::NonStrict) ==
           std::vector<std::size_t>({1, 2, 3}));
}

void testExhaustively() {
    for (std::size_t size = 0; size <= 8; ++size) {
        std::size_t array_count = 1;
        for (std::size_t index = 0; index < size; ++index) {
            array_count *= 3;
        }

        for (std::size_t code = 0; code < array_count; ++code) {
            std::size_t remaining = code;
            std::vector<int> values(size);
            for (std::size_t index = 0; index < size; ++index) {
                values[index] = static_cast<int>(remaining % 3) - 1;
                remaining /= 3;
            }
            checkAgainstBrute(values);
        }
    }
}

void testRandomDifferential() {
    std::mt19937 rng(0x57ACU);
    std::uniform_int_distribution<int> size_distribution(0, 100);
    std::uniform_int_distribution<int> value_distribution(-5, 5);

    for (int iteration = 0; iteration < 5'000; ++iteration) {
        const int size = size_distribution(rng);
        std::vector<int> values(static_cast<std::size_t>(size));
        for (int& value : values) {
            value = value_distribution(rng);
        }
        checkAgainstBrute(values);
    }
}

struct AbsoluteLess {
    bool operator()(int left, int right) const {
        return std::abs(left) < std::abs(right);
    }
};

void testCustomComparator() {
    const std::vector<int> values = {-3, 2, -2, 4};
    const auto actual = cp_library::nearestPreviousLess(
        values, cp_library::MonotonicStrictness::NonStrict, AbsoluteLess{});
    assert(actual == std::vector<std::size_t>({4, 4, 1, 2}));
}

}  // namespace

int main() {
    checkAgainstBrute({});
    testExplicitDuplicates();
    testExhaustively();
    testRandomDifferential();
    testCustomComparator();
    return 0;
}
