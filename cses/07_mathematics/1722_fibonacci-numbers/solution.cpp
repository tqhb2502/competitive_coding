#include <iostream>
#include <utility>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;

pair<long long, long long> fibonacci_pair(unsigned long long n) {
    if (n == 0) {
        return {0, 1};
    }

    const auto [a, b] = fibonacci_pair(n >> 1ULL);
    const long long even = a * ((2 * b % MOD - a + MOD) % MOD) % MOD;
    const long long odd = (a * a % MOD + b * b % MOD) % MOD;

    if (n & 1ULL) {
        return {odd, (even + odd) % MOD};
    }
    return {even, odd};
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned long long n;
    cin >> n;
    cout << fibonacci_pair(n).first << '\n';
    return 0;
}
