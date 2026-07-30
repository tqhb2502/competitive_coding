#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        int heap_count;
        std::cin >> heap_count;
        // Người thứ hai thắng khi và chỉ khi MỌI đống đều chẵn.
        bool all_even = true;
        for (int heap = 0; heap < heap_count; ++heap) {
            long long coins;
            std::cin >> coins;
            // Chỉ cần một đống lẻ là người thứ nhất thắng.
            if ((coins & 1LL) != 0) {
                all_even = false;
            }
        }
        std::cout << (all_even ? "second" : "first") << '\n';
    }
    return 0;
}
