#include <cstdint>
#include <iostream>

namespace {

constexpr long long MOD = 1'000'000'007LL;

struct Matrix {
    long long value[2][2]{};
};

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

Matrix power(Matrix base, long long exponent) {
    Matrix result{};
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

long long countForPrime(long long length, int exponent) {
    if (length == 0) {
        return 1;
    }
    const Matrix transition{{{1, exponent}, {1, 0}}};
    const Matrix raised = power(transition, length - 1);
    const long long first = exponent + 1LL;
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
        if (remaining > 1) {
            answer = answer * countForPrime(n, 1) % MOD;
        }
        std::cout << answer << '\n';
    }
    return 0;
}
