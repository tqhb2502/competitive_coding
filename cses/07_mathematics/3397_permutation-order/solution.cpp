#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Tiền xử lý giai thừa: factorial[i] = i! cho i = 0..20 (tối đa 20! ~ 2.4e18).
    std::vector<std::uint64_t> factorial(21, 1);
    for (int value = 1; value <= 20; ++value) {
        factorial[value] = factorial[value - 1] *
                           static_cast<std::uint64_t>(value);
    }

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        int type, n;
        std::cin >> type >> n;
        // available giữ các phần tử 1..n còn lại, luôn theo thứ tự tăng dần.
        std::vector<int> available(n);
        std::iota(available.begin(), available.end(), 1);

        if (type == 1) {
            // Loại 1: giải mã biểu diễn giai thừa của rank-1 thành hoán vị.
            std::uint64_t rank;
            std::cin >> rank;
            --rank;  // đổi sang đánh số từ 0
            for (int remaining = n; remaining >= 1; --remaining) {
                // block = số hoán vị của các phần tử còn lại sau khi chọn 1 phần tử.
                const std::uint64_t block = factorial[remaining - 1];
                // index = chỉ số phần tử được chọn trong danh sách đã sắp xếp.
                const std::size_t index = static_cast<std::size_t>(rank / block);
                rank %= block;
                if (remaining != n) {
                    std::cout << ' ';
                }
                std::cout << available[index];
                available.erase(available.begin() + static_cast<std::ptrdiff_t>(index));
            }
            std::cout << '\n';
        } else {
            // Loại 2: mã hóa hoán vị thành thứ hạng.
            std::uint64_t rank = 0;
            for (int position = 0; position < n; ++position) {
                int value;
                std::cin >> value;
                // smaller = số phần tử còn lại nhỏ hơn value = chỉ số của value.
                const auto iterator = std::find(
                    available.begin(), available.end(), value
                );
                const auto smaller = std::distance(available.begin(), iterator);
                // Mỗi phần tử nhỏ hơn đứng trước ứng với factorial[...] hoán vị đứng trước.
                rank += static_cast<std::uint64_t>(smaller) *
                        factorial[n - position - 1];
                available.erase(iterator);
            }
            std::cout << rank + 1 << '\n';  // đổi lại về đánh số từ 1
        }
    }
    return 0;
}
