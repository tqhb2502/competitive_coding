#include <iostream>

using namespace std;

constexpr long long MOD = 1'000'000'007;

long long modular_power(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent /= 2;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;
    cout << modular_power(2, n) << '\n';
    return 0;
}
