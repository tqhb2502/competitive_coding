#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using u64 = std::uint64_t;

// Kiểm tra value có phải số chính phương (perfect square) hay không.
// Lấy căn xấp xỉ rồi hiệu chỉnh lên/xuống để tránh sai số dấu phẩy động.
bool is_square(const u64 value) {
    u64 root = static_cast<u64>(std::sqrt(static_cast<long double>(value)));
    while ((root + 1) * (root + 1) <= value) {
        ++root;
    }
    while (root * root > value) {
        --root;
    }
    return root * root == value;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc tất cả truy vấn và tìm giá trị lớn nhất để giới hạn phạm vi sàng.
    int tests;
    std::cin >> tests;
    std::vector<u64> values(tests);
    u64 maximum_value = 0;
    for (u64& value : values) {
        std::cin >> value;
        if (value > maximum_value) {
            maximum_value = value;
        }
    }

    // Cần phân tích thừa số của 4n+1, nên chỉ cần các số nguyên tố tới sqrt(4n+1).
    const u64 maximum_factorized = 4 * maximum_value + 1;
    int sieve_limit = static_cast<int>(
        std::sqrt(static_cast<long double>(maximum_factorized))
    );
    while (static_cast<u64>(sieve_limit + 1) * (sieve_limit + 1) <=
           maximum_factorized) {
        ++sieve_limit;
    }
    while (static_cast<u64>(sieve_limit) * sieve_limit > maximum_factorized) {
        --sieve_limit;
    }

    // Sàng nguyên tố (sieve) để lấy danh sách số nguyên tố dùng chia thử.
    std::vector<char> composite(sieve_limit + 1, false);
    std::vector<int> primes;
    for (int value = 2; value <= sieve_limit; ++value) {
        if (!composite[value]) {
            primes.push_back(value);
            if (value <= sieve_limit / value) {
                for (int multiple = value * value; multiple <= sieve_limit;
                     multiple += value) {
                    composite[multiple] = true;
                }
            }
        }
    }

    for (const u64 value : values) {
        // Đáp án 1: n là số tam giác  <=>  8n+1 là số chính phương.
        if (is_square(8 * value + 1)) {
            std::cout << 1 << '\n';
            continue;
        }

        // Đáp án 2: 4n+1 là tổng của hai bình phương  <=>  mọi ước nguyên tố
        // p == 3 (mod 4) của 4n+1 đều có số mũ chẵn (sum of two squares theorem).
        // Chia thử 4n+1 bằng các số nguyên tố đã sàng.
        u64 remaining = 4 * value + 1;
        bool two_triangles = true;
        for (const int prime : primes) {
            const u64 divisor = static_cast<u64>(prime);
            if (divisor > remaining / divisor) {
                break;
            }
            if (remaining % divisor != 0) {
                continue;
            }
            // Đếm số mũ của thừa số nguyên tố hiện tại.
            int exponent = 0;
            do {
                remaining /= divisor;
                ++exponent;
            } while (remaining % divisor == 0);
            if (prime % 4 == 3 && (exponent & 1) != 0) {
                two_triangles = false;
                break;
            }
        }
        // Phần dư còn lại (nếu > 1) là số nguyên tố với số mũ 1; nếu nó ==3 mod4
        // thì vi phạm điều kiện tổng hai bình phương.
        if (two_triangles && remaining % 4 == 3) {
            two_triangles = false;
        }
        // Còn lại: đáp án 3 (Gauss bảo đảm luôn đủ với 3 số tam giác).
        std::cout << (two_triangles ? 2 : 3) << '\n';
    }
    return 0;
}
