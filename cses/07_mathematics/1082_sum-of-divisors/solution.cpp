#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr long long MOD = 1'000'000'007LL;
    constexpr long long INVERSE_TWO = 500'000'004LL;

    long long n;
    cin >> n;

    long long answer = 0;
    long long left = 1;
    while (left <= n) {
        const long long quotient = n / left;
        const long long right = n / quotient;
        const long long terms = (right - left + 1) % MOD;
        const long long endpoints = (left + right) % MOD;
        const long long interval_sum = endpoints * terms % MOD * INVERSE_TWO % MOD;
        answer = (answer + quotient % MOD * interval_sum) % MOD;
        left = right + 1;
    }

    cout << answer << '\n';
}
