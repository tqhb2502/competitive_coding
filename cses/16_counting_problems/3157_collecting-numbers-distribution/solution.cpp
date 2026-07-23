#include <iostream>

using namespace std;

constexpr long long MOD = 1'000'000'007;
constexpr int MAX_N = 5000;

// ways[k] = số hoán vị cần đúng k lượt = số Eulerian A(size, k-1)
long long ways[MAX_N + 1];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Cơ sở: hoán vị độ dài 1 luôn cần đúng 1 lượt
    ways[1] = 1;

    // Truy hồi số Eulerian khi chèn phần tử lớn nhất vào hoán vị của 1..size-1
    // dp[k] = k * dp[k] + (size - k + 1) * dp[k-1]; duyệt k giảm dần để dùng chung mảng
    for (int size = 2; size <= n; ++size) {
        for (int rounds = size; rounds >= 1; --rounds) {
            ways[rounds] = (rounds * ways[rounds]
                            + static_cast<long long>(size - rounds + 1)
                                  * ways[rounds - 1])
                           % MOD;
        }
    }

    // In số hoán vị ứng với từng số lượt k = 1..n
    for (int rounds = 1; rounds <= n; ++rounds) {
        cout << ways[rounds] << '\n';
    }
}
