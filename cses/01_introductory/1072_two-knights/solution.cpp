#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    for (long long k = 1; k <= n; ++k) {
        const long long square_count = k * k;
        const long long total_pairs = square_count * (square_count - 1) / 2;
        const long long attacking_pairs = 4 * (k - 1) * (k - 2);
        std::cout << total_pairs - attacking_pairs << '\n';
    }

    return 0;
}
