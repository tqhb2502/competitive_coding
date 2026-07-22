#include <array>
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr long long MOD = 1'000'000'007LL;
    std::string s;
    std::cin >> s;

    long long subsequences = 1;  // Includes the empty subsequence.
    std::array<long long, 26> previous_count{};
    std::array<bool, 26> seen{};

    for (const char character : s) {
        const std::size_t letter = static_cast<std::size_t>(character - 'a');
        long long next = 2 * subsequences % MOD;
        if (seen[letter]) {
            next = (next - previous_count[letter] + MOD) % MOD;
        }
        previous_count[letter] = subsequences;
        seen[letter] = true;
        subsequences = next;
    }

    std::cout << (subsequences - 1 + MOD) % MOD << '\n';
    return 0;
}
