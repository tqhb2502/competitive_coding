#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Với mỗi bàn cờ k x k (k từ 1 đến n) áp dụng công thức đếm bù trừ.
    for (long long k = 1; k <= n; ++k) {
        // Số ô trên bàn cờ và tổng số cách chọn 2 ô bất kỳ: C(k*k, 2).
        const long long square_count = k * k;
        const long long total_pairs = square_count * (square_count - 1) / 2;
        // Số cặp Mã tấn công nhau: 4 * (k-1) * (k-2).
        const long long attacking_pairs = 4 * (k - 1) * (k - 2);
        // Kết quả = tổng số cặp - số cặp tấn công nhau.
        std::cout << total_pairs - attacking_pairs << '\n';
    }

    return 0;
}
