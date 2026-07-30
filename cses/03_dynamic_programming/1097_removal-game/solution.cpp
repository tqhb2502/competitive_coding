#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<long long> values(n);
    for (long long& value : values) {
        std::cin >> value;
    }

    const long long total =
        std::accumulate(values.begin(), values.end(), 0LL);

    // dp[left] = hiệu điểm tối ưu (người tới lượt trừ đối thủ) của đoạn bắt đầu
    // tại left với độ dài đang xét. Khởi tạo với độ dài 1: dp[i][i] = a[i].
    std::vector<long long> dp = values;

    // Quy hoạch động theo độ dài đoạn tăng dần để mọi đoạn con ngắn hơn đã có sẵn.
    for (int length = 2; length <= n; ++length) {
        for (int left = 0; left + length <= n; ++left) {
            const int right = left + length - 1;
            // Chọn lấy đầu (values[left]) hoặc lấy cuối (values[right]);
            // phần còn lại đối thủ đi trước nên hiệu điểm bị đảo dấu.
            dp[left] = std::max(values[left] - dp[left + 1],
                                values[right] - dp[left]);
        }
    }

    // Điểm người 1 = (tổng + hiệu điểm) / 2; luôn chia hết vì cùng tính chẵn lẻ.
    const long long first_player_score = (total + dp[0]) / 2;
    std::cout << first_player_score << '\n';
    return 0;
}
