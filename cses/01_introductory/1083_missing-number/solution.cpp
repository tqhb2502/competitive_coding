#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n;
    std::cin >> n;

    // Bắt đầu từ tổng đầy đủ S = 1 + 2 + ... + n = n*(n+1)/2,
    // rồi trừ dần từng số được cho. Phần còn lại chính là số bị thiếu (S - T).
    long long remaining_sum = n * (n + 1) / 2;
    for (long long i = 0; i < n - 1; ++i) {
        long long value;
        std::cin >> value;
        remaining_sum -= value;
    }

    std::cout << remaining_sum << '\n';
    return 0;
}
