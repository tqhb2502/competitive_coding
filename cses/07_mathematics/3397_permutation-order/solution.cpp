#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::uint64_t> factorial(21, 1);
    for (int value = 1; value <= 20; ++value) {
        factorial[value] = factorial[value - 1] *
                           static_cast<std::uint64_t>(value);
    }

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        int type, n;
        std::cin >> type >> n;
        std::vector<int> available(n);
        std::iota(available.begin(), available.end(), 1);

        if (type == 1) {
            std::uint64_t rank;
            std::cin >> rank;
            --rank;
            for (int remaining = n; remaining >= 1; --remaining) {
                const std::uint64_t block = factorial[remaining - 1];
                const std::size_t index = static_cast<std::size_t>(rank / block);
                rank %= block;
                if (remaining != n) {
                    std::cout << ' ';
                }
                std::cout << available[index];
                available.erase(available.begin() + static_cast<std::ptrdiff_t>(index));
            }
            std::cout << '\n';
        } else {
            std::uint64_t rank = 0;
            for (int position = 0; position < n; ++position) {
                int value;
                std::cin >> value;
                const auto iterator = std::find(
                    available.begin(), available.end(), value
                );
                const auto smaller = std::distance(available.begin(), iterator);
                rank += static_cast<std::uint64_t>(smaller) *
                        factorial[n - position - 1];
                available.erase(iterator);
            }
            std::cout << rank + 1 << '\n';
        }
    }
    return 0;
}
