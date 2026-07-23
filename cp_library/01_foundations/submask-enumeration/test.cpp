#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>

#include "algorithm.hpp"

namespace {

template <typename Mask>
std::vector<Mask> enumerate(Mask mask) {
    std::vector<Mask> result;
    cp_library::forEachSubmask(mask, [&](Mask submask) {
        result.push_back(submask);
    });
    return result;
}

template <typename Mask>
std::vector<Mask> enumerateNonzero(Mask mask) {
    std::vector<Mask> result;
    cp_library::forEachNonzeroSubmask(mask, [&](Mask submask) {
        result.push_back(submask);
    });
    return result;
}

std::vector<std::uint32_t> bruteEnumerate(std::uint32_t mask) {
    std::vector<std::uint32_t> result;
    for (std::uint32_t candidate = mask;; --candidate) {
        if ((candidate & ~mask) == 0U) {
            result.push_back(candidate);
        }
        if (candidate == 0U) {
            break;
        }
    }
    return result;
}

std::size_t popcount(std::uint64_t mask) {
    std::size_t count = 0;
    while (mask != 0U) {
        mask &= mask - 1U;
        ++count;
    }
    return count;
}

void testZeroMask() {
    assert(enumerate<std::uint32_t>(0U) == std::vector<std::uint32_t>({0U}));
    assert(enumerateNonzero<std::uint32_t>(0U).empty());
    assert(cp_library::isSubmask<std::uint32_t>(0U, 0U));
}

void testSafeBitConstruction() {
    constexpr std::uint64_t high_bit =
        std::uint64_t{1} << (std::numeric_limits<std::uint64_t>::digits - 1);
    static_assert(cp_library::singleBitOrZero<std::uint64_t>(0) == 1U,
                  "bit zero must exist");
    static_assert(cp_library::singleBitOrZero<std::uint64_t>(63) == high_bit,
                  "highest uint64_t bit must exist");
    static_assert(cp_library::singleBitOrZero<std::uint64_t>(64) == 0U,
                  "out-of-range shift must return zero");
    static_assert(cp_library::singleBitOrZero<std::uint32_t>(32) == 0U,
                  "out-of-range shift must return zero");
}

void testKnownOrderWithHighBit() {
    const std::uint64_t high = cp_library::singleBitOrZero<std::uint64_t>(63);
    const std::uint64_t low = cp_library::singleBitOrZero<std::uint64_t>(5);
    assert(enumerate(high | low) ==
           std::vector<std::uint64_t>({high | low, high, low, 0U}));
}

void testExhaustively() {
    constexpr std::uint32_t limit = std::uint32_t{1} << 12;
    for (std::uint32_t mask = 0; mask < limit; ++mask) {
        const auto actual = enumerate(mask);
        const auto expected = bruteEnumerate(mask);
        assert(actual == expected);

        auto expected_nonzero = expected;
        expected_nonzero.pop_back();
        assert(enumerateNonzero(mask) == expected_nonzero);
    }
}

void testRandomWideMasks() {
    std::mt19937_64 rng(0x5AB5E7ULL);
    std::uniform_int_distribution<int> bit_distribution(0, 63);

    for (int iteration = 0; iteration < 500; ++iteration) {
        std::uint64_t mask = 0;
        for (int pick = 0; pick < 15; ++pick) {
            mask |= cp_library::singleBitOrZero<std::uint64_t>(
                static_cast<std::size_t>(bit_distribution(rng)));
        }

        const auto submasks = enumerate(mask);
        const std::size_t expected_count =
            std::size_t{1} << popcount(mask);
        assert(submasks.size() == expected_count);
        assert(submasks.front() == mask);
        assert(submasks.back() == 0U);

        for (std::size_t index = 0; index < submasks.size(); ++index) {
            assert(cp_library::isSubmask(submasks[index], mask));
            if (index > 0) {
                assert(submasks[index - 1] > submasks[index]);
            }
        }

        const auto nonzero = enumerateNonzero(mask);
        assert(nonzero.size() + 1 == submasks.size());
        for (std::size_t index = 0; index < nonzero.size(); ++index) {
            assert(nonzero[index] == submasks[index]);
        }
    }
}

}  // namespace

int main() {
    testZeroMask();
    testSafeBitConstruction();
    testKnownOrderWithHighBit();
    testExhaustively();
    testRandomWideMasks();
    return 0;
}
