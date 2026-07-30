#include <iostream>

using namespace std;

constexpr long long MOD = 1'000'000'007;
constexpr int MAX_N = 5000;

long long factorial[MAX_N + 1];
long long inverseFactorial[MAX_N + 1];
long long inverse[MAX_N + 1];
long long powerOfN[MAX_N + 1];
long long binomialPolynomial[MAX_N];  // hệ số của Q_r(y) = binom(y+r, r) theo bậc y
long long sumPolynomial[MAX_N];       // đa thức tổng: sum_r w_r * Q_r(y)

// Luỹ thừa nhanh theo modulo (modpow), dùng để lấy nghịch đảo qua Fermat
long long modPow(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent & 1LL) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1LL;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Tiền xử lí: giai thừa, luỹ thừa của n, và nghịch đảo modulo
    factorial[0] = 1;
    inverseFactorial[0] = 1;
    inverse[0] = 1;
    inverse[1] = 1;
    powerOfN[0] = 1;
    for (int i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * i % MOD;
        powerOfN[i] = powerOfN[i - 1] * n % MOD;
    }
    // Nghịch đảo của giai thừa, tính lùi từ inverseFactorial[n]
    inverseFactorial[n] = modPow(factorial[n], MOD - 2);
    for (int i = n; i >= 1; --i) {
        inverseFactorial[i - 1] = inverseFactorial[i] * i % MOD;
    }
    // Nghịch đảo của từng số 1..n bằng truy hồi tuyến tính
    for (int i = 2; i <= n; ++i) {
        inverse[i] = MOD - (MOD / i) * inverse[MOD % i] % MOD;
    }

    // Q_0(y) = 1
    binomialPolynomial[0] = 1;

    for (int r = 0; r < n; ++r) {
        // Trọng số weight w_r = n^(n-1-r) / (n-1-r)!
        const int exponent = n - 1 - r;
        const long long weight = powerOfN[exponent] * inverseFactorial[exponent] % MOD;

        // Cộng w_r * Q_r(y) vào đa thức tổng, theo từng bậc y
        for (int degree = 0; degree <= r; ++degree) {
            sumPolynomial[degree] += weight * binomialPolynomial[degree] % MOD;
            if (sumPolynomial[degree] >= MOD) {
                sumPolynomial[degree] -= MOD;
            }
        }

        // Cập nhật Q_{r+1}(y) = Q_r(y) * (y + r + 1)/(r + 1)
        // Duyệt bậc giảm dần: q_mới[d] = q_cũ[d] + q_cũ[d-1]/(r+1)
        if (r + 1 < n) {
            const long long inverseDenominator = inverse[r + 1];
            for (int degree = r + 1; degree >= 1; --degree) {
                binomialPolynomial[degree] +=
                    binomialPolynomial[degree - 1] * inverseDenominator % MOD;
                if (binomialPolynomial[degree] >= MOD) {
                    binomialPolynomial[degree] -= MOD;
                }
            }
        }
    }

    // Nhân thừa số (n-1)!; thừa số y làm dịch bậc nên hệ số y^k lấy ở vị trí k-1
    const long long scale = factorial[n - 1];
    for (int components = 1; components <= n; ++components) {
        cout << scale * sumPolynomial[components - 1] % MOD << '\n';
    }
}
