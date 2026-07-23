#include <iostream>

int main() {
    // Tăng tốc I/O
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Các trường hợp đặc biệt: n = 1 luôn hợp lệ; n = 2 và n = 3 vô nghiệm
    if (n == 1) {
        std::cout << "1\n";
        return 0;
    }
    if (n == 2 || n == 3) {
        std::cout << "NO SOLUTION\n";
        return 0;
    }

    // Với n >= 4: in các số CHẴN trước, rồi tới các số LẺ.
    // Trong mỗi nhóm hai số liên tiếp hơn kém nhau 2, và điểm nối (4 với 1)
    // có hiệu 3 nên không có cặp kề nhau nào hiệu bằng 1.
    bool first = true;

    // Nhóm số chẵn: 2, 4, 6, ...
    for (int value = 2; value <= n; value += 2) {
        if (!first) {
            std::cout << ' ';
        }
        std::cout << value;
        first = false;
    }

    // Nhóm số lẻ: 1, 3, 5, ...
    for (int value = 1; value <= n; value += 2) {
        if (!first) {
            std::cout << ' ';
        }
        std::cout << value;
        first = false;
    }
    std::cout << '\n';

    return 0;
}
