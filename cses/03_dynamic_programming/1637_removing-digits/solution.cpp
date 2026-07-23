#include <algorithm>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Greedy: mỗi bước trừ n đi chữ số lớn nhất của nó (tối ưu, trùng với DP).
    int steps = 0;
    while (n > 0) {
        // Tìm chữ số lớn nhất trong biểu diễn thập phân của n.
        int largest_digit = 0;
        for (int value = n; value > 0; value /= 10) {
            largest_digit = std::max(largest_digit, value % 10);
        }
        n -= largest_digit;
        ++steps;
    }

    std::cout << steps << '\n';
    return 0;
}
