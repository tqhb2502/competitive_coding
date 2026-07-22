#include <array>
#include <iostream>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;
constexpr size_t SIZE = 6;
using Matrix = array<array<long long, SIZE>, SIZE>;

Matrix multiply(const Matrix& first, const Matrix& second) {
    Matrix product{};
    for (size_t row = 0; row < SIZE; ++row) {
        for (size_t middle = 0; middle < SIZE; ++middle) {
            if (first[row][middle] == 0) {
                continue;
            }
            for (size_t column = 0; column < SIZE; ++column) {
                product[row][column] =
                    (product[row][column] + first[row][middle] * second[middle][column]) % MOD;
            }
        }
    }
    return product;
}

Matrix matrix_power(Matrix base, long long exponent) {
    Matrix result{};
    for (size_t i = 0; i < SIZE; ++i) {
        result[i][i] = 1;
    }
    while (exponent > 0) {
        if (exponent & 1LL) {
            result = multiply(result, base);
        }
        base = multiply(base, base);
        exponent >>= 1LL;
    }
    return result;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    constexpr array<long long, SIZE> initial = {16, 8, 4, 2, 1, 1};
    if (n <= 5) {
        constexpr array<long long, 6> small = {1, 1, 2, 4, 8, 16};
        cout << small[static_cast<size_t>(n)] << '\n';
        return 0;
    }

    Matrix transition{};
    transition[0].fill(1);
    for (size_t i = 1; i < SIZE; ++i) {
        transition[i][i - 1] = 1;
    }

    const Matrix power = matrix_power(transition, n - 5);
    long long answer = 0;
    for (size_t i = 0; i < SIZE; ++i) {
        answer = (answer + power[0][i] * initial[i]) % MOD;
    }
    cout << answer << '\n';
}
