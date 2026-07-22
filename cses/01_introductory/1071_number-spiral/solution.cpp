#include <algorithm>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int test_count;
    std::cin >> test_count;

    while (test_count--) {
        long long y;
        long long x;
        std::cin >> y >> x;

        const long long layer = std::max(y, x);
        long long answer;

        if (layer % 2 == 0) {
            if (x == layer) {
                answer = (layer - 1) * (layer - 1) + y;
            } else {
                answer = layer * layer - x + 1;
            }
        } else {
            if (y == layer) {
                answer = (layer - 1) * (layer - 1) + x;
            } else {
                answer = layer * layer - y + 1;
            }
        }

        std::cout << answer << '\n';
    }

    return 0;
}
