#include <algorithm>
#include <iostream>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;

long long mod_pow(long long base, long long exponent) {
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
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    const int total_positions = n + m - 1;
    const int chosen = min(m, n - 1);

    long long numerator = 1;
    long long denominator = 1;
    for (int i = 1; i <= chosen; ++i) {
        numerator = numerator * (total_positions - chosen + i) % MOD;
        denominator = denominator * i % MOD;
    }

    const long long answer = numerator * mod_pow(denominator, MOD - 2) % MOD;
    cout << answer << '\n';
    return 0;
}
