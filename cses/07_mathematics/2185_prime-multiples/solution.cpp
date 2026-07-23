#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n;
    int k;
    std::cin >> n >> k;
    std::vector<long long> primes(static_cast<std::size_t>(k));
    for (long long& prime : primes) {
        std::cin >> prime;
    }

    long long answer = 0;
    // Mỗi phần tử lưu (tích của một tập con, số phần tử của tập con đó);
    // khởi tạo bằng tập rỗng (tích = 1, lực lượng = 0).
    std::vector<std::pair<long long, int>> subsets{{1, 0}};
    for (const long long prime : primes) {
        const std::size_t old_size = subsets.size();
        // Mở rộng mọi tập con hiện có bằng cách thêm số nguyên tố prime.
        for (std::size_t i = 0; i < old_size; ++i) {
            const auto [product, cardinality] = subsets[i];
            // Cắt tỉa: nếu product * prime > n thì đóng góp bằng 0.
            // Dùng so sánh product > n / prime để tránh tràn số khi nhân.
            if (product > n / prime) {
                continue;
            }
            const long long new_product = product * prime;
            const int new_cardinality = cardinality + 1;
            subsets.emplace_back(new_product, new_cardinality);

            // Bao hàm - loại trừ: tập con lẻ cộng, tập con chẵn trừ.
            if ((new_cardinality & 1) != 0) {
                answer += n / new_product;
            } else {
                answer -= n / new_product;
            }
        }
    }

    std::cout << answer << '\n';
    return 0;
}
