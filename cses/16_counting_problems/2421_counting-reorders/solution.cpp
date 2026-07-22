#include <algorithm>
#include <array>
#include <iostream>
#include <string>

using namespace std;

constexpr int MOD = 1'000'000'007;
constexpr int MAX_N = 5000;

long long factorial[MAX_N + 1];
long long inverseFactorial[MAX_N + 1];
long long ways[MAX_N + 1];
long long nextWays[MAX_N + 1];
long long blockWays[MAX_N + 1];

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

    string input;
    cin >> input;
    const int n = static_cast<int>(input.size());

    array<int, 26> frequency{};
    for (char letter : input) {
        ++frequency[static_cast<size_t>(letter - 'a')];
    }

    factorial[0] = 1;
    inverseFactorial[0] = 1;
    for (int i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * i % MOD;
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

    ways[0] = 1;
    int used = 0;

    for (int count : frequency) {
        if (count == 0) {
            continue;
        }

        for (int blocks = 1; blocks <= count; ++blocks) {
            blockWays[blocks] = combination(count - 1, blocks - 1)
                                * inverseFactorial[blocks] % MOD;
        }

        fill(nextWays, nextWays + used + count + 1, 0);
        for (int oldBlocks = 0; oldBlocks <= used; ++oldBlocks) {
            if (ways[oldBlocks] == 0) {
                continue;
            }
            for (int blocks = 1; blocks <= count; ++blocks) {
                long long& destination = nextWays[oldBlocks + blocks];
                destination += ways[oldBlocks] * blockWays[blocks] % MOD;
                if (destination >= MOD) {
                    destination -= MOD;
                }
            }
        }

        used += count;
        copy(nextWays, nextWays + used + 1, ways);
    }

    long long answer = 0;
    for (int blocks = 1; blocks <= n; ++blocks) {
        const long long term = ways[blocks] * factorial[blocks] % MOD;
        if ((n - blocks) & 1) {
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
