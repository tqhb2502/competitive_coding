#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    constexpr long long MOD = 1'000'000'007LL;
    if (n == 1) {
        cout << 0 << '\n';
        return 0;
    }

    long long two_back = 0;  // D(1)
    long long one_back = 1;  // D(2)
    for (long long size = 3; size <= n; ++size) {
        const long long current = (size - 1) * ((one_back + two_back) % MOD) % MOD;
        two_back = one_back;
        one_back = current;
    }

    cout << one_back << '\n';
    return 0;
}
