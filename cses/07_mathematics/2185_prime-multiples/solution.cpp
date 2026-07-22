#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n;
    int k;
    std::cin >> n >> k;
    std::vector<long long> primes(static_cast<std::size_t>(k));
    for (long long& prime : primes) {
        std::cin >> prime;
    }

    long long answer = 0;
    // Each entry stores a nonempty/empty subset product and its cardinality.
    std::vector<std::pair<long long, int>> subsets{{1, 0}};
    for (const long long prime : primes) {
        const std::size_t old_size = subsets.size();
        for (std::size_t i = 0; i < old_size; ++i) {
            const auto [product, cardinality] = subsets[i];
            if (product > n / prime) {
                continue;
            }
            const long long new_product = product * prime;
            const int new_cardinality = cardinality + 1;
            subsets.emplace_back(new_product, new_cardinality);

            if ((new_cardinality & 1) != 0) {
                answer += n / new_product;
            } else {
                answer -= n / new_product;
            }
        }
    }

    std::cout << answer << '\n';
    return 0;
}
