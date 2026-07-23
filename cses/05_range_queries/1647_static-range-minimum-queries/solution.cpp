#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t q = 0;
    std::cin >> n >> q;

    // Số tầng của Sparse Table: levels sao cho 2^(levels-1) <= n < 2^levels.
    std::size_t levels = 1;
    while ((std::size_t{1} << levels) <= n) {
        ++levels;
    }

    // table[k][i] = min của đoạn có độ dài 2^k bắt đầu tại i, tức [i, i + 2^k - 1].
    std::vector<std::vector<int>> table(levels, std::vector<int>(n));
    for (int& value : table[0]) {
        std::cin >> value;
    }
    // Xây dựng theo quy hoạch động trên k: ghép hai đoạn 2^(k-1) kề nhau.
    for (std::size_t level = 1; level < levels; ++level) {
        const std::size_t half = std::size_t{1} << (level - 1);
        const std::size_t length = std::size_t{1} << level;
        for (std::size_t start = 0; start + length <= n; ++start) {
            table[level][start] =
                std::min(table[level - 1][start], table[level - 1][start + half]);
        }
    }

    // Bảng log2 nguyên dựng sẵn: floor_log[len] = floor(log2(len)).
    std::vector<std::size_t> floor_log(n + 1, 0);
    for (std::size_t length = 2; length <= n; ++length) {
        floor_log[length] = floor_log[length / 2] + 1;
    }

    for (std::size_t query = 0; query < q; ++query) {
        std::size_t left = 0;
        std::size_t right = 0;
        std::cin >> left >> right;
        --left;  // đổi truy vấn về 0-indexed
        --right;
        // Phủ [left, right] bằng hai đoạn độ dài 2^level (có thể chồng lấn).
        const std::size_t length = right - left + 1;
        const std::size_t level = floor_log[length];
        const std::size_t block_length = std::size_t{1} << level;
        std::cout << std::min(table[level][left],
                              table[level][right - block_length + 1])
                  << '\n';
    }
    return 0;
}
