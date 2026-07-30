#include <algorithm>
#include <iostream>

int main() {
    // Tăng tốc nhập/xuất
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int test_count;
    std::cin >> test_count;

    while (test_count--) {
        long long y;
        long long x;
        std::cin >> y >> x;

        // Lớp chữ "L" chứa ô (y, x) chính là max(y, x)
        const long long layer = std::max(y, x);
        long long answer;

        // Hướng tăng của lớp đảo chiều tùy theo lớp chẵn hay lẻ
        if (layer % 2 == 0) {
            if (x == layer) {
                // Cột phải: giá trị tăng khi y tăng
                answer = (layer - 1) * (layer - 1) + y;
            } else {
                // Hàng dưới (y == layer): giá trị giảm khi x tăng
                answer = layer * layer - x + 1;
            }
        } else {
            if (y == layer) {
                // Hàng dưới: giá trị tăng khi x tăng
                answer = (layer - 1) * (layer - 1) + x;
            } else {
                // Cột phải (x == layer): giá trị giảm khi y tăng
                answer = layer * layer - y + 1;
            }
        }

        std::cout << answer << '\n';
    }

    return 0;
}
