#include <iostream>
#include <cstddef>
#include <vector>

namespace {
constexpr long long MOD = 1'000'000'007LL;

// Lũy thừa modular: tính base^exponent mod MOD bằng bình phương liên tiếp.
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

    // n lẻ thì không thể có dãy ngoặc hợp lệ nào.
    if ((n & 1U) != 0U) {
        std::cout << 0 << '\n';
        return 0;
    }

    // Tiền xử lý giai thừa: factorial[k] = k! mod MOD.
    std::vector<long long> factorial(n + 1, 1);
    for (std::size_t i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * static_cast<long long>(i) % MOD;
    }

    // Nghịch đảo giai thừa: dùng định lý Fermat cho phần tử cuối rồi truy hồi
    // ngược inv_fact[k-1] = inv_fact[k] * k % MOD.
    std::vector<long long> inverse_factorial(n + 1, 1);
    inverse_factorial[n] = mod_power(factorial[n], MOD - 2);
    for (std::size_t i = n; i > 0; --i) {
        inverse_factorial[i - 1] =
            inverse_factorial[i] * static_cast<long long>(i) % MOD;
    }

    // Số cặp ngoặc m = n / 2; đáp án là số Catalan thứ m:
    // Catalan(m) = (2m)! / (m! * (m+1)!), với 2m = n.
    const std::size_t pairs = n / 2;
    const long long answer = factorial[n] * inverse_factorial[pairs] % MOD *
                             inverse_factorial[pairs + 1] % MOD;
    std::cout << answer << '\n';
    return 0;
}
