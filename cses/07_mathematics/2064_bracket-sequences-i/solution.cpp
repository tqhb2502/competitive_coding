#include <iostream>
#include <cstddef>
#include <vector>

namespace {
constexpr long long MOD = 1'000'000'007LL;

long long mod_power(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if ((exponent & 1LL) != 0) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1LL;
    }
    return result;
}
}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n;
    std::cin >> n;
    if ((n & 1U) != 0U) {
        std::cout << 0 << '\n';
        return 0;
    }

    std::vector<long long> factorial(n + 1, 1);
    std::vector<long long> inverse_factorial(n + 1, 1);
    for (std::size_t i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * static_cast<long long>(i) % MOD;
    }
    inverse_factorial[n] = mod_power(factorial[n], MOD - 2);
    for (std::size_t i = n; i > 0; --i) {
        inverse_factorial[i - 1] =
            inverse_factorial[i] * static_cast<long long>(i) % MOD;
    }

    const std::size_t pairs = n / 2;
    const long long answer = factorial[n] * inverse_factorial[pairs] % MOD *
                             inverse_factorial[pairs + 1] % MOD;
    std::cout << answer << '\n';
    return 0;
}
