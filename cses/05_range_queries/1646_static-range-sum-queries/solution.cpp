#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    // Tăng tốc I/O
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t q = 0;
    std::cin >> n >> q;

    // Dựng mảng tổng tiền tố: prefix[i] = x[1] + ... + x[i], prefix[0] = 0
    // Dùng long long để tổng không bị tràn số
    std::vector<long long> prefix(n + 1, 0);
    for (std::size_t i = 1; i <= n; ++i) {
        long long value = 0;
        std::cin >> value;
        prefix[i] = prefix[i - 1] + value;
    }

    // Mỗi truy vấn (a, b): tổng đoạn [a, b] = prefix[b] - prefix[a-1], tốn O(1)
    for (std::size_t query = 0; query < q; ++query) {
        std::size_t left = 0;
        std::size_t right = 0;
        std::cin >> left >> right;
        std::cout << prefix[right] - prefix[left - 1] << '\n';
    }
    return 0;
}
