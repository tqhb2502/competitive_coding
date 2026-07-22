#include <iostream>
#include <cstddef>
#include <unordered_set>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // The last losing heap size for this game is 1222.  Computing somewhat
    // beyond it reproduces all losing positions without hard-coding the list.
    constexpr std::size_t LIMIT = 1300;
    std::vector<int> grundy(LIMIT + 1, 0);
    for (std::size_t coins = 3; coins <= LIMIT; ++coins) {
        std::unordered_set<int> reachable;
        for (std::size_t left = 1; left < coins - left; ++left) {
            reachable.insert(grundy[left] ^ grundy[coins - left]);
        }

        int mex = 0;
        while (reachable.count(mex) != 0U) {
            ++mex;
        }
        grundy[coins] = mex;
    }

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        std::size_t coins;
        std::cin >> coins;
        const bool losing = coins <= LIMIT && grundy[coins] == 0;
        std::cout << (losing ? "second" : "first") << '\n';
    }
    return 0;
}
