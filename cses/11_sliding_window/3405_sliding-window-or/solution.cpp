#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace {

class Generator {
public:
    Generator(const std::uint64_t initial, const std::uint64_t multiplier,
              const std::uint64_t increment, const std::uint64_t modulus)
        : current_(initial),
          multiplier_(multiplier),
          increment_(increment),
          modulus_(modulus),
          reciprocal_(static_cast<std::uint64_t>(
              (static_cast<__uint128_t>(1) << 64) / modulus
          )) {}

    std::uint32_t next() {
        const std::uint32_t result = static_cast<std::uint32_t>(current_);
        const std::uint64_t product = multiplier_ * current_ + increment_;
        const std::uint64_t quotient = static_cast<std::uint64_t>(
            (static_cast<__uint128_t>(product) * reciprocal_) >> 64
        );

        current_ = product - quotient * modulus_;
        if (current_ >= modulus_) {
            current_ -= modulus_;
        }
        return result;
    }

private:
    std::uint64_t current_;
    const std::uint64_t multiplier_;
    const std::uint64_t increment_;
    const std::uint64_t modulus_;
    const std::uint64_t reciprocal_;
};

void build_suffix_or(std::uint32_t* const block, const int length) {
    std::uint32_t suffix = 0;
    for (int index = length - 1; index >= 0; --index) {
        suffix |= block[index];
        block[index] = suffix;
    }
}

}  // namespace

int main() {
    int n;
    int k;
    unsigned long long input_x;
    unsigned long long input_a;
    unsigned long long input_b;
    unsigned long long input_c;

    if (std::scanf("%d %d", &n, &k) != 2) {
        return 0;
    }
    if (std::scanf("%llu %llu %llu %llu", &input_x, &input_a,
                   &input_b, &input_c) != 4) {
        return 0;
    }

    const std::uint32_t x = static_cast<std::uint32_t>(input_x);
    const std::uint64_t a = static_cast<std::uint64_t>(input_a);
    const std::uint64_t b = static_cast<std::uint64_t>(input_b);
    const std::uint64_t c = static_cast<std::uint64_t>(input_c);

    // When c = 1, the sequence is x, 0, 0, ... . Only the first window
    // contains x, so the xor of all window ors is x for every valid k.
    if (c == 1) {
        std::printf("%u\n", static_cast<unsigned int>(x));
        return 0;
    }

    Generator generator(x, a, b, c);
    std::uint32_t answer = 0;

    if (k == 1) {
        for (int index = 0; index < n; ++index) {
            answer ^= generator.next();
        }
        std::printf("%u\n", static_cast<unsigned int>(answer));
        return 0;
    }

    if (k == n) {
        for (int index = 0; index < n; ++index) {
            answer |= generator.next();
        }
        std::printf("%u\n", static_cast<unsigned int>(answer));
        return 0;
    }

    std::uint32_t* const block =
        new std::uint32_t[static_cast<std::size_t>(k)];

    // The first aligned block forms the first window.
    for (int offset = 0; offset < k; ++offset) {
        block[offset] = generator.next();
        answer |= block[offset];
    }
    build_suffix_or(block, k);

    int generated = k;
    while (generated < n) {
        const int remaining = n - generated;
        const int length = remaining < k ? remaining : k;
        std::uint32_t prefix = 0;

        for (int offset = 0; offset < length; ++offset) {
            const std::uint32_t value = generator.next();
            prefix |= value;

            // block[offset + 1] is still the suffix of the preceding block.
            // At the last position of a full block, the window is exactly the
            // current block and prefix alone is its OR.
            if (offset + 1 < k) {
                answer ^= block[offset + 1] | prefix;
            } else {
                answer ^= prefix;
            }
            block[offset] = value;
        }

        generated += length;
        if (generated < n) {
            build_suffix_or(block, k);
        }
    }

    delete[] block;
    std::printf("%u\n", static_cast<unsigned int>(answer));
    return 0;
}
