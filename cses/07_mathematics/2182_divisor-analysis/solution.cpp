#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>

namespace {
constexpr long long MOD = 1'000'000'007LL;
constexpr long long PHI = MOD - 1;  // dùng cho Fermat: x^e = x^(e mod PHI) khi gcd(x, MOD) = 1

// Lũy thừa nhanh theo modulo.
long long mod_power(long long base, long long exponent, long long modulus) {
    long long result = 1;
    base %= modulus;
    while (exponent > 0) {
        if ((exponent & 1LL) != 0) {
            result = result * base % modulus;
        }
        base = base * base % modulus;
        exponent >>= 1LL;
    }
    return result;
}
}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<std::pair<long long, long long>> factors(
        static_cast<std::size_t>(n));
    // D chẵn <=> tồn tại ít nhất một số mũ k_i lẻ (khi đó k_i + 1 chẵn).
    bool divisor_count_is_even = false;
    for (auto& [prime, exponent] : factors) {
        std::cin >> prime >> exponent;
        if ((exponent & 1LL) != 0) {
            divisor_count_is_even = true;
        }
    }

    long long divisor_count = 1;
    long long divisor_sum = 1;
    for (const auto& [prime, exponent] : factors) {
        // (1) Số lượng ước: tích (k_i + 1) mod MOD.
        divisor_count = divisor_count * ((exponent + 1) % MOD) % MOD;

        // (2) Tổng ước: tích chuỗi hình học (x^{k+1} - 1) / (x - 1).
        // Vì 2 <= x <= 1e6 < MOD nên x - 1 khác 0 mod MOD, nghịch đảo luôn tồn tại.
        const long long numerator =
            (mod_power(prime, exponent + 1, MOD) + MOD - 1) % MOD;
        const long long inverse_denominator =
            mod_power(prime - 1, MOD - 2, MOD);
        divisor_sum = divisor_sum * numerator % MOD * inverse_denominator % MOD;
    }

    // (3) Tích ước: tích x_i^{k_i * D / 2}, với D = số ước (nguyên).
    // Luôn giữ số mũ ở dạng nguyên trước khi rút gọn theo mod PHI, vì PHI chẵn nên
    // 2 không khả nghịch mod PHI -> không thể chia 2 trực tiếp mod PHI.
    long long divisor_product = 1;
    if (divisor_count_is_even) {
        // D chẵn: chia đôi đúng một thừa số (k_j + 1) chẵn để có D/2 nguyên,
        // rồi số mũ mỗi thừa số là E_i = k_i * (D/2).
        long long half_count = 1;
        bool halved = false;
        for (const auto& [prime, exponent] : factors) {
            (void)prime;
            long long factor = exponent + 1;
            if (!halved && (factor & 1LL) == 0) {
                factor /= 2;
                halved = true;
            }
            half_count = half_count * (factor % PHI) % PHI;
        }

        for (const auto& [prime, exponent] : factors) {
            const long long total_exponent =
                (exponent % PHI) * half_count % PHI;
            divisor_product = divisor_product *
                              mod_power(prime, total_exponent, MOD) % MOD;
        }
    } else {
        // D lẻ (mọi k_i chẵn): dùng E_i = (k_i / 2) * D.
        long long count_mod_phi = 1;
        for (const auto& [prime, exponent] : factors) {
            (void)prime;
            count_mod_phi = count_mod_phi * ((exponent + 1) % PHI) % PHI;
        }
        for (const auto& [prime, exponent] : factors) {
            const long long total_exponent =
                ((exponent / 2) % PHI) * count_mod_phi % PHI;
            divisor_product = divisor_product *
                              mod_power(prime, total_exponent, MOD) % MOD;
        }
    }

    std::cout << divisor_count << ' ' << divisor_sum << ' '
              << divisor_product << '\n';
    return 0;
}
