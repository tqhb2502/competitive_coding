#include <cstdint>
#include <iostream>

namespace {

constexpr long long MOD = 1'000'000'007LL;

// Ma trận 2x2 dùng cho hệ thức truy hồi F_m = F_{m-1} + e * F_{m-2}.
struct Matrix {
    long long value[2][2]{};
};

// Nhân hai ma trận 2x2 theo modulo.
Matrix multiply(const Matrix& first, const Matrix& second) {
    Matrix result{};
    for (int row = 0; row < 2; ++row) {
        for (int middle = 0; middle < 2; ++middle) {
            for (int column = 0; column < 2; ++column) {
                result.value[row][column] =
                    (result.value[row][column] +
                     first.value[row][middle] * second.value[middle][column]) % MOD;
            }
        }
    }
    return result;
}

// Lũy thừa ma trận nhanh: base^exponent trong O(log exponent).
Matrix power(Matrix base, long long exponent) {
    Matrix result{};  // Khởi tạo bằng ma trận đơn vị.
    result.value[0][0] = 1;
    result.value[1][1] = 1;
    while (exponent > 0) {
        if ((exponent & 1LL) != 0) {
            result = multiply(result, base);
        }
        base = multiply(base, base);
        exponent >>= 1LL;
    }
    return result;
}

// Đếm số chuỗi số mũ hợp lệ độ dài "length" cho một nguyên tố có số mũ "exponent".
// F_0 = 1, F_1 = exponent + 1, F_m = F_{m-1} + exponent * F_{m-2}.
long long countForPrime(long long length, int exponent) {
    if (length == 0) {
        return 1;
    }
    // Ma trận chuyển đưa (F_{m-1}, F_{m-2}) sang (F_m, F_{m-1}).
    const Matrix transition{{{1, exponent}, {1, 0}}};
    const Matrix raised = power(transition, length - 1);
    const long long first = exponent + 1LL;  // F_1
    // Áp dụng lên vector khởi tạo (F_1, F_0) để lấy F_length.
    return (raised.value[0][0] * first + raised.value[0][1]) % MOD;
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int tests = 0;
    std::cin >> tests;
    while (tests-- > 0) {
        long long n = 0;
        int k = 0;
        std::cin >> n >> k;

        long long answer = 1;
        int remaining = k;
        // Phân tích thừa số nguyên tố của k; mỗi p^e đóng góp độc lập vào đáp án.
        for (int prime = 2; static_cast<long long>(prime) * prime <= remaining;
             prime += (prime == 2 ? 1 : 2)) {
            if (remaining % prime != 0) {
                continue;
            }
            int exponent = 0;
            while (remaining % prime == 0) {
                remaining /= prime;
                ++exponent;
            }
            answer = answer * countForPrime(n, exponent) % MOD;
        }
        // Thừa số nguyên tố còn lại lớn hơn sqrt(k) có số mũ bằng 1.
        if (remaining > 1) {
            answer = answer * countForPrime(n, 1) % MOD;
        }
        std::cout << answer << '\n';
    }
    return 0;
}
