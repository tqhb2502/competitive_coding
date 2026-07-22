#include <cstdint>
#include <iostream>

using namespace std;

constexpr int MOD = 1'000'000'007;
constexpr int MAX_N = 500;

long long factorial[MAX_N + 1];
long long inverseFactorial[MAX_N + 1];
long long powerOfTwo[MAX_N + 1];
long long allTournaments[MAX_N + 1];
long long strong[MAX_N + 1];
int transition[MAX_N + 1][MAX_N + 1];
int distribution[MAX_N + 1][MAX_N + 1];

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
    powerOfTwo[0] = 1;
    for (int i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * i % MOD;
        powerOfTwo[i] = powerOfTwo[i - 1] * 2 % MOD;
    }
    inverseFactorial[n] = modPow(factorial[n], MOD - 2);
    for (int i = n; i >= 1; --i) {
        inverseFactorial[i - 1] = inverseFactorial[i] * i % MOD;
    }

    const auto combination = [&](int total, int chosen) -> long long {
        return factorial[total] * inverseFactorial[chosen] % MOD
               * inverseFactorial[total - chosen] % MOD;
    };

    allTournaments[0] = 1;
    for (int size = 1; size <= n; ++size) {
        allTournaments[size] = allTournaments[size - 1] * powerOfTwo[size - 1] % MOD;

        long long nonStrong = 0;
        for (int firstSize = 1; firstSize < size; ++firstSize) {
            long long term = combination(size, firstSize) * strong[firstSize] % MOD;
            term = term * allTournaments[size - firstSize] % MOD;
            nonStrong += term;
            if (nonStrong >= MOD) {
                nonStrong -= MOD;
            }
        }
        strong[size] = allTournaments[size] - nonStrong;
        if (strong[size] < 0) {
            strong[size] += MOD;
        }
    }

    for (int total = 1; total <= n; ++total) {
        for (int componentSize = 1; componentSize <= total; ++componentSize) {
            transition[total][componentSize] = static_cast<int>(
                combination(total, componentSize) * strong[componentSize] % MOD);
        }
    }

    distribution[0][0] = 1;

    for (int components = 1; components <= n; ++components) {
        for (int total = components; total <= n; ++total) {
            const int maxLastSize = total - components + 1;
            uint64_t sum = 0;
            int batchSize = 0;

            for (int lastSize = 1; lastSize <= maxLastSize; ++lastSize) {
                sum += static_cast<uint64_t>(transition[total][lastSize])
                       * static_cast<uint64_t>(
                           distribution[total - lastSize][components - 1]);
                ++batchSize;
                if (batchSize == 8) {
                    sum %= MOD;
                    batchSize = 0;
                }
            }
            distribution[total][components] = static_cast<int>(sum % MOD);
        }
    }

    for (int components = 1; components <= n; ++components) {
        cout << distribution[n][components] << '\n';
    }
}
