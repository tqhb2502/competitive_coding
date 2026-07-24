#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b;
    cin >> a >> b;

    // dp[i][j] = số lần cắt tối thiểu để chia hình chữ nhật i × j thành các hình
    // vuông. Khởi tạo 0 nên đường chéo dp[i][i] = 0 (hình vuông) đã có sẵn.
    const int limit = max(a, b);
    vector<vector<int>> dp(limit + 1, vector<int>(limit + 1, 0));

    // Chỉ tính nửa tam giác trên (i < j) rồi soi gương, vì dp[i][j] = dp[j][i].
    for (int i = 1; i <= limit; ++i) {
        for (int j = i + 1; j <= limit; ++j) {
            int best = numeric_limits<int>::max();

            // Cắt theo cạnh dài j (giữ nguyên i): dp[i][k] + dp[i][j-k] + 1.
            // Do đối xứng vị trí cắt nên chỉ cần duyệt k tới j/2.
            for (int cut = 1; cut <= j / 2; ++cut) {
                best = min(best, dp[i][cut] + dp[i][j - cut] + 1);
            }

            // Cắt theo cạnh dài i (giữ nguyên j): dp[k][j] + dp[i-k][j] + 1. Nhờ
            // đối xứng dp[k][j] = dp[j][k] nên đọc theo hàng j (dp[j][cut]) đã được
            // soi gương từ các i nhỏ hơn; cũng chỉ cần duyệt cut tới i/2.
            for (int cut = 1; cut <= i / 2; ++cut) {
                best = min(best, dp[j][cut] + dp[j][i - cut] + 1);
            }

            dp[i][j] = best;
            dp[j][i] = best;  // soi gương
        }
    }

    // Đáp án là dp[a][b] (trường hợp a == b cho ngay 0 nhờ khởi tạo).
    cout << dp[a][b] << '\n';
    return 0;
}
