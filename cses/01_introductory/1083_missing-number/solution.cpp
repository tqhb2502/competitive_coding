#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n;
    std::cin >> n;

    long long remaining_sum = n * (n + 1) / 2;
    for (long long i = 0; i < n - 1; ++i) {
        long long value;
        std::cin >> value;
        remaining_sum -= value;
    }

    std::cout << remaining_sum << '\n';
    return 0;
}
