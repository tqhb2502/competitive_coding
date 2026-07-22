#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        int n;
        std::cin >> n;
        long long nim_sum = 0;
        for (int i = 0; i < n; ++i) {
            long long heap;
            std::cin >> heap;
            nim_sum ^= heap;
        }
        std::cout << (nim_sum != 0 ? "first" : "second") << '\n';
    }
    return 0;
}
