#include <iostream>
#include <cstddef>
#include <string>
#include <vector>

namespace {
constexpr long long MOD = 1'000'000'007LL;

// Lũy thừa nhanh theo modulo, dùng để tính nghịch đảo (modular inverse) qua Fermat.
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

    int n;
    std::string prefix;
    std::cin >> n >> prefix;

    // Bước 1: n lẻ thì không thể cân bằng nên đáp án là 0.
    if ((n & 1) != 0) {
        std::cout << 0 << '\n';
        return 0;
    }

    // Bước 2: quét prefix, mỗi '(' là +1, mỗi ')' là -1. Nếu balance xuống dưới
    // 0 thì prefix không hợp lệ, đáp án là 0.
    int balance = 0;
    for (const char bracket : prefix) {
        balance += (bracket == '(' ? 1 : -1);
        if (balance < 0) {
            std::cout << 0 << '\n';
            return 0;
        }
    }

    // Bước 3: còn remaining = n - k bước để đi từ độ cao balance về 0. Nếu không
    // đủ bước hoặc chênh lệch lẻ thì không có đường đi hợp lệ.
    const int remaining = n - static_cast<int>(prefix.size());
    if (remaining < balance || ((remaining - balance) & 1) != 0) {
        std::cout << 0 << '\n';
        return 0;
    }

    // Tiền xử lý factorial và inverse factorial theo modulo tới n.
    const std::size_t factorial_size = static_cast<std::size_t>(n) + 1;
    std::vector<long long> factorial(factorial_size, 1);
    std::vector<long long> inverse_factorial(factorial_size, 1);
    for (std::size_t i = 1; i < factorial_size; ++i) {
        factorial[i] = factorial[i - 1] * static_cast<long long>(i) % MOD;
    }
    const std::size_t last = factorial_size - 1;
    inverse_factorial[last] = mod_power(factorial[last], MOD - 2);
    for (std::size_t i = last; i > 0; --i) {
        inverse_factorial[i - 1] =
            inverse_factorial[i] * static_cast<long long>(i) % MOD;
    }

    // Tổ hợp C(total, chosen), trả 0 nếu chosen nằm ngoài khoảng [0, total].
    const auto combination = [&](int total, int chosen) -> long long {
        if (chosen < 0 || chosen > total) {
            return 0;
        }
        const std::size_t total_index = static_cast<std::size_t>(total);
        const std::size_t chosen_index = static_cast<std::size_t>(chosen);
        return factorial[total_index] * inverse_factorial[chosen_index] % MOD *
               inverse_factorial[total_index - chosen_index] % MOD;
    };

    // Reflection principle: số đường tự do trừ số đường chạm mức -1.
    // C(m, (m-b)/2) - C(m, (m+b+2)/2); ở đây up_steps = (m-b)/2 nên
    // (m+b+2)/2 = m - up_steps + 1, tương đương chọn up_steps - 1 (do đối xứng
    // C(m, r) = C(m, m-r)).
    const int up_steps = (remaining - balance) / 2;
    const long long unrestricted = combination(remaining, up_steps);
    const long long crosses_boundary = combination(remaining, up_steps - 1);
    const long long answer =
        (unrestricted - crosses_boundary + MOD) % MOD;
    std::cout << answer << '\n';
    return 0;
}
