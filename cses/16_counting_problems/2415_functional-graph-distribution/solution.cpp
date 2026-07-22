#include <iostream>

using namespace std;

constexpr long long MOD = 1'000'000'007;
constexpr int MAX_N = 5000;

long long factorial[MAX_N + 1];
long long inverseFactorial[MAX_N + 1];
long long inverse[MAX_N + 1];
long long powerOfN[MAX_N + 1];
long long binomialPolynomial[MAX_N];
long long sumPolynomial[MAX_N];

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

    factorial[0] = 1;
    inverseFactorial[0] = 1;
    inverse[0] = 1;
    inverse[1] = 1;
    powerOfN[0] = 1;
    for (int i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * i % MOD;
        powerOfN[i] = powerOfN[i - 1] * n % MOD;
    }
    inverseFactorial[n] = modPow(factorial[n], MOD - 2);
    for (int i = n; i >= 1; --i) {
        inverseFactorial[i - 1] = inverseFactorial[i] * i % MOD;
    }
    for (int i = 2; i <= n; ++i) {
        inverse[i] = MOD - (MOD / i) * inverse[MOD % i] % MOD;
    }

    binomialPolynomial[0] = 1;

    for (int r = 0; r < n; ++r) {
        const int exponent = n - 1 - r;
        const long long weight = powerOfN[exponent] * inverseFactorial[exponent] % MOD;

        for (int degree = 0; degree <= r; ++degree) {
            sumPolynomial[degree] += weight * binomialPolynomial[degree] % MOD;
            if (sumPolynomial[degree] >= MOD) {
                sumPolynomial[degree] -= MOD;
            }
        }

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

    const long long scale = factorial[n - 1];
    for (int components = 1; components <= n; ++components) {
        cout << scale * sumPolynomial[components - 1] % MOD << '\n';
    }
}
