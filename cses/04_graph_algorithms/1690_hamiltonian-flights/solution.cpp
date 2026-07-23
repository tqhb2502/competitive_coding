#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // edge_count[a][b] = số cạnh có hướng a -> b. Giữ bội số để xử lý luôn các
    // chuyến bay lặp lại giữa cùng một cặp thành phố.
    std::vector<std::vector<int>> edge_count(n, std::vector<int>(n, 0));
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        ++edge_count[a - 1][b - 1];
    }

    constexpr int MOD = 1'000'000'007;

    // Điểm xuất phát (thành phố 1) và điểm đích (thành phố n) là cố định, nên
    // bitmask chỉ cần lưu n-2 thành phố trung gian. Trường hợp n == 2 không có
    // đỉnh trung gian: đáp án chính là số cạnh trực tiếp từ 1 đến n.
    const int internal_count = n - 2;
    if (internal_count == 0) {
        std::cout << edge_count[0][n - 1] % MOD << '\n';
        return 0;
    }

    // dp[mask][last] = số đường đi Hamilton xuất phát từ thành phố 1, đã thăm
    // đúng tập thành phố trung gian `mask`, và hiện đang kết thúc tại thành phố
    // trung gian `last`.
    const int mask_count = 1 << internal_count;
    std::vector<int> dp(
        static_cast<std::size_t>(mask_count) * internal_count,
        0
    );

    // Duyệt mask tăng dần: previous_mask = mask bỏ bit last luôn nhỏ hơn mask nên
    // đã được tính trước, bảo đảm không phụ thuộc vòng.
    for (int mask = 1; mask < mask_count; ++mask) {
        int possible_last = mask;
        while (possible_last != 0) {
            // Lấy ra một đỉnh trung gian `last` làm điểm kết thúc của trạng thái.
            const int last_bit = possible_last & -possible_last;
            possible_last ^= last_bit;
            const int last = __builtin_ctz(static_cast<unsigned>(last_bit));
            const int previous_mask = mask ^ last_bit;

            long long ways = 0;
            if (previous_mask == 0) {
                // `last` là thành phố trung gian đầu tiên: đi thẳng từ thành phố 1.
                ways = edge_count[0][last + 1];
            } else {
                // Cộng dồn từ mọi đỉnh liền trước `previous` trong previous_mask
                // có cạnh previous -> last.
                int previous_cities = previous_mask;
                while (previous_cities != 0) {
                    const int previous_bit =
                        previous_cities & -previous_cities;
                    previous_cities ^= previous_bit;
                    const int previous = __builtin_ctz(
                        static_cast<unsigned>(previous_bit)
                    );
                    ways += static_cast<long long>(
                                dp[static_cast<std::size_t>(previous_mask) *
                                       internal_count + previous]
                            ) * edge_count[previous + 1][last + 1];
                }
            }

            dp[static_cast<std::size_t>(mask) * internal_count + last] =
                static_cast<int>(ways % MOD);
        }
    }

    // Kết thúc: từ trạng thái đã thăm hết mọi thành phố trung gian, đi cạnh cuối
    // last -> thành phố đích n.
    const int full_mask = mask_count - 1;
    long long answer = 0;
    for (int last = 0; last < internal_count; ++last) {
        answer += static_cast<long long>(
                      dp[static_cast<std::size_t>(full_mask) * internal_count +
                         last]
                  ) * edge_count[last + 1][n - 1];
    }

    std::cout << answer % MOD << '\n';
    return 0;
}
