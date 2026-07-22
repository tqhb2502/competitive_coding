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

    void subtract(const BigUnsigned& other) {
        std::int64_t borrow = 0;
        for (std::size_t index = 0; index < digits_.size(); ++index) {
            std::int64_t current =
                static_cast<std::int64_t>(digits_[index]) - borrow;
            if (index < other.digits_.size()) {
                current -= other.digits_[index];
            }
            if (current < 0) {
                current += static_cast<std::int64_t>(BIG_BASE);
                borrow = 1;
            } else {
                borrow = 0;
            }
            digits_[index] = static_cast<std::uint32_t>(current);
        }
        normalize();
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

BigUnsigned integer_power(const int base, const int exponent) {
    BigUnsigned result(1);
    for (int power = 0; power < exponent; ++power) {
        result.multiply(static_cast<std::uint64_t>(base));
    }
    return result;
}

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

    int children, maximum_candies;
    std::cin >> children >> maximum_candies;

    // E[max] = k - sum_{value=1}^{k-1} (value/k)^n.
    const BigUnsigned denominator = integer_power(maximum_candies, children);
    BigUnsigned numerator = denominator;
    numerator.multiply(static_cast<std::uint64_t>(maximum_candies));
    for (int value = 1; value < maximum_candies; ++value) {
        numerator.subtract(integer_power(value, children));
    }

    print_half_even(
        numerator, denominator,
        static_cast<std::uint64_t>(maximum_candies) * DECIMAL_SCALE
    );
    return 0;
}
