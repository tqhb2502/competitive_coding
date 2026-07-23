#include <iostream>

int main() {
    // Bật fast I/O để in nhanh khi dãy dài
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Dùng long long vì giá trị trung gian có thể tới ~2^32, vượt phạm vi int
    long long n;
    std::cin >> n;

    // Mô phỏng trực tiếp dãy Collatz: in giá trị hiện tại rồi sinh giá trị kế tiếp
    while (true) {
        std::cout << n;
        if (n == 1) {
            break;  // Đã in ra giá trị 1: kết thúc dãy
        }
        std::cout << ' ';  // Dấu cách ngăn cách giữa các giá trị
        if (n % 2 == 0) {
            n /= 2;         // n chẵn: chia đôi
        } else {
            n = 3 * n + 1;  // n lẻ: nhân ba cộng một
        }
    }
    std::cout << '\n';

    return 0;
}
