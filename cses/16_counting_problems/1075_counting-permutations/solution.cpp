#include <algorithm>
#include <iostream>

using namespace std;

constexpr int MOD = 1'000'000'007;
constexpr int MAX_N = 1000;

long long factorial[MAX_N + 1];
long long inverseFactorial[MAX_N + 1];
long long powerOfTwo[MAX_N + 1];

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
        if (chosen < 0 || chosen > total) {
            return 0;
        }
        return factorial[total] * inverseFactorial[chosen] % MOD
               * inverseFactorial[total - chosen] % MOD;
    };

    long long answer = factorial[n];
    for (int edges = 1; edges < n; ++edges) {
        long long edgeSets = 0;
        const int maxRuns = min(edges, n - edges);

        for (int runs = 1; runs <= maxRuns; ++runs) {
            long long ways = combination(edges - 1, runs - 1);
            ways = ways * combination(n - edges, runs) % MOD;
            ways = ways * powerOfTwo[runs] % MOD;
            edgeSets += ways;
            if (edgeSets >= MOD) {
                edgeSets -= MOD;
            }
        }

        const long long term = edgeSets * factorial[n - edges] % MOD;
        if (edges & 1) {
            answer -= term;
            if (answer < 0) {
                answer += MOD;
            }
        } else {
            answer += term;
            if (answer >= MOD) {
                answer -= MOD;
            }
        }
    }

    cout << answer << '\n';
}
