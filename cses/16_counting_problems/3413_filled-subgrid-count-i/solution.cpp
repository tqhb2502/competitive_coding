#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    // dp[c] = cạnh lớn nhất của hình vuông đồng màu có góc dưới phải ở cột c
    std::vector<int> dp(n, 0);
    std::vector<std::int64_t> answer(k, 0);  // đáp án cho từng chữ cái
    std::string previous(n, '#');            // hàng phía trên
    std::string current;

    for (int row = 0; row < n; ++row) {
        std::cin >> current;
        int left_dp = 0;      // dp của ô bên trái
        int diagonal_dp = 0;  // dp của ô chéo trên-trái

        for (int column = 0; column < n; ++column) {
            const int upper_dp = dp[column];  // dp của ô phía trên
            int square_size = 1;              // mỗi ô luôn có hình vuông cạnh 1

            // Nếu ô hiện tại giống cả ba ô trên, trái và chéo trên-trái
            // thì mở rộng được hình vuông thêm một đơn vị
            if (row > 0 && column > 0 && current[column] == previous[column] &&
                current[column] == current[column - 1] &&
                current[column] == previous[column - 1]) {
                square_size = 1 + std::min({upper_dp, left_dp, diagonal_dp});
            }

            dp[column] = square_size;
            left_dp = square_size;
            diagonal_dp = upper_dp;

            // Có đúng square_size hình vuông đồng màu kết thúc tại ô này
            answer[current[column] - 'A'] += square_size;
        }

        previous = current;  // hàng hiện tại trở thành hàng trên cho bước sau
    }

    for (const std::int64_t count : answer) {
        std::cout << count << '\n';
    }
}
