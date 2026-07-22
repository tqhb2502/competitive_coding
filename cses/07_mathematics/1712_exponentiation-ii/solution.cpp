#include <iostream>

using namespace std;

namespace {
long long mod_power(long long base, long long exponent, long long modulus) {
    long long result = 1;
    base %= modulus;
    while (exponent > 0) {
        if (exponent & 1LL) {
            result = result * base % modulus;
        }
        base = base * base % modulus;
        exponent >>= 1LL;
    }
    return result;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr long long MOD = 1'000'000'007LL;
    constexpr long long PHI = MOD - 1;

    int queries;
    cin >> queries;
    while (queries--) {
        long long a, b, c;
        cin >> a >> b >> c;

        if (a == 0) {
            const bool exponent_is_zero = (b == 0 && c > 0);
            cout << (exponent_is_zero ? 1 : 0) << '\n';
            continue;
        }

        const long long reduced_exponent = mod_power(b, c, PHI);
        cout << mod_power(a, reduced_exponent, MOD) << '\n';
    }
}
