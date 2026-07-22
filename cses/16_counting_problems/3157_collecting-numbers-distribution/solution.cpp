#include <iostream>

using namespace std;

constexpr long long MOD = 1'000'000'007;
constexpr int MAX_N = 5000;

long long ways[MAX_N + 1];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    ways[1] = 1;

    for (int size = 2; size <= n; ++size) {
        for (int rounds = size; rounds >= 1; --rounds) {
            ways[rounds] = (rounds * ways[rounds]
                            + static_cast<long long>(size - rounds + 1)
                                  * ways[rounds - 1])
                           % MOD;
        }
    }

    for (int rounds = 1; rounds <= n; ++rounds) {
        cout << ways[rounds] << '\n';
    }
}
