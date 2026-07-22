#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        int heap_count;
        std::cin >> heap_count;
        bool all_even = true;
        for (int heap = 0; heap < heap_count; ++heap) {
            long long coins;
            std::cin >> coins;
            if ((coins & 1LL) != 0) {
                all_even = false;
            }
        }
        std::cout << (all_even ? "second" : "first") << '\n';
    }
    return 0;
}
