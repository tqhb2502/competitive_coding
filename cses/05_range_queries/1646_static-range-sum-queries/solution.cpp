// Static Range Sum Queries - CSES 1646
// Prefix sums answer every immutable range-sum query in O(1).

#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t q = 0;
    std::cin >> n >> q;

    std::vector<long long> prefix(n + 1, 0);
    for (std::size_t i = 1; i <= n; ++i) {
        long long value = 0;
        std::cin >> value;
        prefix[i] = prefix[i - 1] + value;
    }

    for (std::size_t query = 0; query < q; ++query) {
        std::size_t left = 0;
        std::size_t right = 0;
        std::cin >> left >> right;
        std::cout << prefix[right] - prefix[left - 1] << '\n';
    }
    return 0;
}
