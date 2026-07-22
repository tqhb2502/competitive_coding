#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr long long MOD = 1'000'000'007LL;

long long modular_power(long long base, int exponent) {
    long long result = 1;
    while (exponent > 0) {
        if ((exponent & 1) != 0) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}
}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::string pattern;
    std::cin >> n >> pattern;
    const int m = static_cast<int>(pattern.size());

    std::vector<int> prefix(static_cast<std::size_t>(m), 0);
    for (int i = 1; i < m; ++i) {
        int matched = prefix[static_cast<std::size_t>(i - 1)];
        while (matched > 0 &&
               pattern[static_cast<std::size_t>(i)] != pattern[static_cast<std::size_t>(matched)]) {
            matched = prefix[static_cast<std::size_t>(matched - 1)];
        }
        if (pattern[static_cast<std::size_t>(i)] ==
            pattern[static_cast<std::size_t>(matched)]) {
            ++matched;
        }
        prefix[static_cast<std::size_t>(i)] = matched;
    }

    std::vector<std::array<int, 26>> automaton(static_cast<std::size_t>(m));
    for (int state = 0; state < m; ++state) {
        for (int letter = 0; letter < 26; ++letter) {
            const char current = static_cast<char>('A' + letter);
            if (current == pattern[static_cast<std::size_t>(state)]) {
                automaton[static_cast<std::size_t>(state)][static_cast<std::size_t>(letter)] =
                    state + 1;
            } else if (state == 0) {
                automaton[0][static_cast<std::size_t>(letter)] = 0;
            } else {
                automaton[static_cast<std::size_t>(state)][static_cast<std::size_t>(letter)] =
                    automaton[static_cast<std::size_t>(prefix[static_cast<std::size_t>(state - 1)])]
                             [static_cast<std::size_t>(letter)];
            }
        }
    }

    std::vector<long long> dp(static_cast<std::size_t>(m), 0);
    std::vector<long long> next(static_cast<std::size_t>(m), 0);
    dp[0] = 1;
    for (int length = 0; length < n; ++length) {
        std::fill(next.begin(), next.end(), 0);
        for (int state = 0; state < m; ++state) {
            const long long ways = dp[static_cast<std::size_t>(state)];
            if (ways == 0) {
                continue;
            }
            for (int letter = 0; letter < 26; ++letter) {
                const int new_state =
                    automaton[static_cast<std::size_t>(state)][static_cast<std::size_t>(letter)];
                if (new_state < m) {
                    long long& destination = next[static_cast<std::size_t>(new_state)];
                    destination += ways;
                    if (destination >= MOD) {
                        destination -= MOD;
                    }
                }
            }
        }
        dp.swap(next);
    }

    long long avoiding = 0;
    for (const long long ways : dp) {
        avoiding += ways;
        if (avoiding >= MOD) {
            avoiding -= MOD;
        }
    }
    const long long answer = (modular_power(26, n) - avoiding + MOD) % MOD;
    std::cout << answer << '\n';
    return 0;
}
