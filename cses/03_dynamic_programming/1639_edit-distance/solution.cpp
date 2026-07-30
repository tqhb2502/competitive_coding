#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string source;
    std::string target;
    std::cin >> source >> target;

    // Chọn chuỗi ngắn hơn làm cột để mảng dp cuộn chỉ rộng m cột.
    if (source.size() < target.size()) {
        std::swap(source, target);
    }

    const int rows = static_cast<int>(source.size());
    const int columns = static_cast<int>(target.size());

    // dp[j] = edit distance giữa i ký tự đầu của source và j ký tự đầu target.
    // Base case hàng 0: dp[j] = j (chèn thêm j ký tự).
    std::vector<int> dp(columns + 1);
    std::iota(dp.begin(), dp.end(), 0);

    for (int row = 1; row <= rows; ++row) {
        // diagonal giữ giá trị dp[i-1][j-1] của ô chéo trước khi bị ghi đè.
        int diagonal = dp[0];
        dp[0] = row;  // dp[i][0] = i (xóa i ký tự)

        for (int column = 1; column <= columns; ++column) {
            const int from_above = dp[column];  // dp[i-1][j]
            if (source[row - 1] == target[column - 1]) {
                dp[column] = diagonal;  // ký tự khớp: không tốn phép nào
            } else {
                // 1 + min(xóa s[i-1], chèn t[j-1], thay thế s[i-1] bằng t[j-1])
                dp[column] = 1 + std::min({from_above,
                                           dp[column - 1],
                                           diagonal});
            }
            diagonal = from_above;  // ô chéo cho cột kế tiếp
        }
    }

    std::cout << dp[columns] << '\n';
    return 0;
}
