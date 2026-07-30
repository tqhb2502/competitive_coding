#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {
constexpr int64_t MOD = 1'000'000'007;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text;
    cin >> text;
    const int length = static_cast<int>(text.size());

    // Độ dài lẻ thì không thể xóa hết.
    if (length % 2 == 1) {
        cout << 0 << '\n';
        return 0;
    }

    // Tiền xử lý hệ số nhị thức C(n,k) tới n/2 bằng tam giác Pascal.
    const int pair_count = length / 2;
    vector<vector<int64_t>> choose(pair_count + 1,
                                   vector<int64_t>(pair_count + 1, 0));
    choose[0][0] = 1;
    for (int n = 1; n <= pair_count; ++n) {
        choose[n][0] = 1;
        choose[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            choose[n][k] = (choose[n - 1][k - 1] + choose[n - 1][k]) % MOD;
        }
    }

    // dp[l][r] = số cách xóa hết đoạn s[l..r] (đoạn có độ dài chẵn).
    // Duyệt theo độ dài đoạn tăng dần để các đoạn con nhỏ hơn đã có sẵn.
    vector<vector<int64_t>> dp(length, vector<int64_t>(length, 0));
    for (int segment_length = 2; segment_length <= length; segment_length += 2) {
        for (int left = 0; left + segment_length <= length; ++left) {
            const int right = left + segment_length - 1;
            int64_t ways = 0;

            // Ghép s[left] với ký tự cùng giá trị s[partner] (cùng parity).
            for (int partner = left + 1; partner <= right; partner += 2) {
                if (text[left] != text[partner]) {
                    continue;
                }

                // Đoạn [left+1, partner-1] phải xóa hết trước khi ghép cặp;
                // đoạn [partner+1, right] xóa độc lập sau đó.
                const int64_t inside =
                    (partner == left + 1) ? 1 : dp[left + 1][partner - 1];
                const int64_t suffix =
                    (partner == right) ? 1 : dp[partner + 1][right];
                const int component_pairs = (partner - left + 1) / 2;

                // Trộn hai dãy phép xóa: C((r-l+1)/2, (partner-l+1)/2) cách.
                int64_t contribution = inside * suffix % MOD;
                contribution = contribution *
                               choose[segment_length / 2][component_pairs] % MOD;
                ways += contribution;
                if (ways >= MOD) {
                    ways -= MOD;
                }
            }
            dp[left][right] = ways;
        }
    }

    cout << dp[0][length - 1] << '\n';
}
