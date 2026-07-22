#include <iostream>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;

long long mod_power(long long base, long long exponent) {
    long long result = 1;
    base %= MOD;
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

    int queries;
    cin >> queries;
    while (queries--) {
        long long a, b;
        cin >> a >> b;
        cout << mod_power(a, b) << '\n';
    }
}
