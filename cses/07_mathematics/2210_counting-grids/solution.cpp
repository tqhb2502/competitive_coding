#include <iostream>

constexpr long long MOD = 1'000'000'007LL;

long long modular_power(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if ((exponent & 1LL) != 0) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent /= 2;
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long size;
    std::cin >> size;
    const long long cells = size * size;

    const long long quarter_turn_orbits =
        size % 2 == 0 ? cells / 4 : (cells + 3) / 4;
    const long long half_turn_orbits =
        size % 2 == 0 ? cells / 2 : (cells + 1) / 2;

    const long long fixed_identity = modular_power(2, cells);
    const long long fixed_quarter = modular_power(2, quarter_turn_orbits);
    const long long fixed_half = modular_power(2, half_turn_orbits);
    const long long total = (
        fixed_identity + 2 * fixed_quarter + fixed_half
    ) % MOD;
    const long long inverse_four = modular_power(4, MOD - 2);
    std::cout << total * inverse_four % MOD << '\n';
    return 0;
}
