#include <iostream>
#include <vector>

constexpr long long MOD = 1'000'000'007LL;

long long modular_power(long long base, long long exponent) {
    long long result = 1;
    base %= MOD;
    while (exponent > 0) {
        if ((exponent & 1LL) != 0) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent /= 2;
    }
    return result;
}

int euler_phi(int value) {
    int result = value;
    int remaining = value;
    for (int factor = 2; factor <= remaining / factor; ++factor) {
        if (remaining % factor != 0) {
            continue;
        }
        while (remaining % factor == 0) {
            remaining /= factor;
        }
        result -= result / factor;
    }
    if (remaining > 1) {
        result -= result / remaining;
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int pearl_count, color_count;
    std::cin >> pearl_count >> color_count;

    std::vector<int> divisors;
    for (int divisor = 1; divisor <= pearl_count / divisor; ++divisor) {
        if (pearl_count % divisor != 0) {
            continue;
        }
        divisors.push_back(divisor);
        if (divisor != pearl_count / divisor) {
            divisors.push_back(pearl_count / divisor);
        }
    }

    long long fixed_sum = 0;
    for (const int divisor : divisors) {
        const long long term =
            static_cast<long long>(euler_phi(divisor)) *
            modular_power(color_count, pearl_count / divisor) % MOD;
        fixed_sum += term;
        if (fixed_sum >= MOD) {
            fixed_sum -= MOD;
        }
    }

    const long long answer =
        fixed_sum * modular_power(pearl_count, MOD - 2) % MOD;
    std::cout << answer << '\n';
    return 0;
}
