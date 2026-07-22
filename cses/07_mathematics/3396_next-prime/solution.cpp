#include <array>
#include <cstdint>
#include <iostream>

using u64 = std::uint64_t;
using u128 = __uint128_t;

u64 modular_multiply(const u64 first, const u64 second, const u64 modulus) {
    return static_cast<u64>(
        static_cast<u128>(first) * second % modulus
    );
}

u64 modular_power(u64 base, u64 exponent, const u64 modulus) {
    u64 result = 1;
    while (exponent > 0) {
        if ((exponent & 1U) != 0U) {
            result = modular_multiply(result, base, modulus);
        }
        base = modular_multiply(base, base, modulus);
        exponent /= 2;
    }
    return result;
}

bool is_prime(const u64 value) {
    if (value < 2) {
        return false;
    }
    for (const u64 prime : std::array<u64, 12>{
             2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (value % prime == 0) {
            return value == prime;
        }
    }

    u64 odd_part = value - 1;
    int powers_of_two = 0;
    while ((odd_part & 1U) == 0U) {
        odd_part /= 2;
        ++powers_of_two;
    }

    constexpr std::array<u64, 7> witnesses{
        2, 325, 9'375, 28'178, 450'775, 9'780'504, 1'795'265'022
    };
    for (const u64 witness : witnesses) {
        if (witness % value == 0) {
            continue;
        }
        u64 power = modular_power(witness % value, odd_part, value);
        if (power == 1 || power == value - 1) {
            continue;
        }

        bool probable_prime = false;
        for (int round = 1; round < powers_of_two; ++round) {
            power = modular_multiply(power, power, value);
            if (power == value - 1) {
                probable_prime = true;
                break;
            }
        }
        if (!probable_prime) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        u64 value;
        std::cin >> value;
        if (value < 2) {
            std::cout << 2 << '\n';
            continue;
        }

        u64 candidate = value + 1;
        if ((candidate & 1U) == 0U) {
            ++candidate;
        }
        while (!is_prime(candidate)) {
            candidate += 2;
        }
        std::cout << candidate << '\n';
    }
    return 0;
}
