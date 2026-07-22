#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {

constexpr std::uint64_t BIG_BASE = 1'000'000'000ULL;
constexpr std::uint64_t DECIMAL_SCALE = 1'000'000ULL;

class BigUnsigned {
public:
    explicit BigUnsigned(std::uint64_t value = 0) {
        do {
            digits_.push_back(static_cast<std::uint32_t>(value % BIG_BASE));
            value /= BIG_BASE;
        } while (value > 0);
    }

    void multiply(const std::uint64_t factor) {
        if (factor == 0) {
            digits_.assign(1, 0);
            return;
        }
        std::uint64_t carry = 0;
        for (std::uint32_t& digit : digits_) {
            const std::uint64_t current =
                static_cast<std::uint64_t>(digit) * factor + carry;
            digit = static_cast<std::uint32_t>(current % BIG_BASE);
            carry = current / BIG_BASE;
        }
        while (carry > 0) {
            digits_.push_back(
                static_cast<std::uint32_t>(carry % BIG_BASE)
            );
            carry /= BIG_BASE;
        }
    }

    void add(const BigUnsigned& other) {
        if (digits_.size() < other.digits_.size()) {
            digits_.resize(other.digits_.size(), 0);
        }
        std::uint64_t carry = 0;
        for (std::size_t index = 0; index < digits_.size(); ++index) {
            std::uint64_t current = digits_[index] + carry;
            if (index < other.digits_.size()) {
                current += other.digits_[index];
            }
            digits_[index] = static_cast<std::uint32_t>(current % BIG_BASE);
            carry = current / BIG_BASE;
        }
        if (carry > 0) {
            digits_.push_back(static_cast<std::uint32_t>(carry));
        }
    }

    [[nodiscard]] BigUnsigned divided_by(const std::uint32_t divisor) const {
        BigUnsigned quotient;
        quotient.digits_.assign(digits_.size(), 0);
        std::uint64_t remainder = 0;
        for (std::size_t index = digits_.size(); index-- > 0;) {
            const std::uint64_t current =
                remainder * BIG_BASE + digits_[index];
            quotient.digits_[index] =
                static_cast<std::uint32_t>(current / divisor);
            remainder = current % divisor;
        }
        quotient.normalize();
        return quotient;
    }

    [[nodiscard]] int compare(const BigUnsigned& other) const {
        if (digits_.size() != other.digits_.size()) {
            return digits_.size() < other.digits_.size() ? -1 : 1;
        }
        for (std::size_t index = digits_.size(); index-- > 0;) {
            if (digits_[index] != other.digits_[index]) {
                return digits_[index] < other.digits_[index] ? -1 : 1;
            }
        }
        return 0;
    }

private:
    void normalize() {
        while (digits_.size() > 1 && digits_.back() == 0) {
            digits_.pop_back();
        }
    }

    std::vector<std::uint32_t> digits_;
};

void print_half_even(const BigUnsigned& numerator,
                     const BigUnsigned& denominator,
                     const std::uint64_t upper_scaled) {
    BigUnsigned scaled = numerator;
    scaled.multiply(DECIMAL_SCALE);

    std::uint64_t low = 0;
    std::uint64_t high = upper_scaled;
    while (low < high) {
        const std::uint64_t middle = low + (high - low + 1) / 2;
        BigUnsigned product = denominator;
        product.multiply(middle);
        if (product.compare(scaled) <= 0) {
            low = middle;
        } else {
            high = middle - 1;
        }
    }

    BigUnsigned twice_scaled = scaled;
    twice_scaled.multiply(2);
    BigUnsigned midpoint = denominator;
    midpoint.multiply(2 * low + 1);
    const int midpoint_comparison = twice_scaled.compare(midpoint);
    if (midpoint_comparison > 0 ||
        (midpoint_comparison == 0 && (low & 1ULL) != 0ULL)) {
        ++low;
    }

    std::cout << low / DECIMAL_SCALE << '.' << std::setw(6)
              << std::setfill('0') << low % DECIMAL_SCALE << '\n';
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> ranges(static_cast<std::size_t>(n));
    for (int& range : ranges) {
        std::cin >> range;
    }

    // D is L^2, where L is the lcm of all ranges.  Build D directly from
    // the largest prime exponent present in any range.
    std::array<int, 101> maximum_exponent{};
    for (const int range : ranges) {
        int remaining = range;
        for (int prime = 2; prime <= remaining / prime; ++prime) {
            int exponent = 0;
            while (remaining % prime == 0) {
                remaining /= prime;
                ++exponent;
            }
            maximum_exponent[static_cast<std::size_t>(prime)] = std::max(
                maximum_exponent[static_cast<std::size_t>(prime)], exponent
            );
        }
        if (remaining > 1) {
            maximum_exponent[static_cast<std::size_t>(remaining)] = std::max(
                maximum_exponent[static_cast<std::size_t>(remaining)], 1
            );
        }
    }

    BigUnsigned denominator(1);
    for (int prime = 2; prime <= 100; ++prime) {
        for (int exponent = 0;
             exponent < 2 * maximum_exponent[static_cast<std::size_t>(prime)];
             ++exponent) {
            denominator.multiply(static_cast<std::uint64_t>(prime));
        }
    }

    BigUnsigned numerator(0);
    for (std::size_t i = 0; i < ranges.size(); ++i) {
        for (std::size_t j = i + 1; j < ranges.size(); ++j) {
            const long long upper = std::min(ranges[j], ranges[i] - 1);
            if (upper <= 0) {
                continue;
            }
            const long long favorable =
                upper * ranges[i] - upper * (upper + 1) / 2;
            const int pair_denominator = ranges[i] * ranges[j];
            BigUnsigned term = denominator.divided_by(
                static_cast<std::uint32_t>(pair_denominator)
            );
            term.multiply(static_cast<std::uint64_t>(favorable));
            numerator.add(term);
        }
    }

    const std::uint64_t maximum_inversions =
        static_cast<std::uint64_t>(n) *
        static_cast<std::uint64_t>(n - 1) / 2;
    print_half_even(
        numerator, denominator, maximum_inversions * DECIMAL_SCALE
    );
    return 0;
}
