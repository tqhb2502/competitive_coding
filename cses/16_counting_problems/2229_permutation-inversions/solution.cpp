#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr int MOD = 1'000'000'007;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int inversions;
    cin >> n >> inversions;

    // Số nghịch thế tối đa và đối xứng: đếm cho k bằng với đếm cho total-k,
    // nên thay k bằng min(k, total-k) để giảm số trạng thái.
    const int maximum_inversions = n * (n - 1) / 2;
    const int target = min(inversions, maximum_inversions - inversions);

    // dp cuộn: previous là hàng i-1, current là hàng i. Cơ sở previous[0]=1: hoán vị {1} có 0 nghịch thế (vòng lặp bắt đầu chèn phần tử 2).
    vector<int> previous(target + 1, 0);
    vector<int> current(target + 1, 0);
    previous[0] = 1;
    int previous_limit = 0;

    // Chèn phần tử lớn nhất length vào hoán vị của 1..length-1, tạo thêm 0..length-1 nghịch thế.
    for (int length = 2; length <= n; ++length) {
        const int current_limit = min(target, length * (length - 1) / 2);
        int window = 0;  // Tổng cửa sổ dp[i-1][j-length+1..j]

        for (int value = 0; value <= current_limit; ++value) {
            // Thêm dp cũ tại value vào cửa sổ.
            if (value <= previous_limit) {
                window += previous[value];
                if (window >= MOD) {
                    window -= MOD;
                }
            }
            // Bỏ dp cũ tại value-length ra khỏi cửa sổ (chỉ giữ length số hạng gần nhất).
            if (value >= length) {
                window -= previous[value - length];
                if (window < 0) {
                    window += MOD;
                }
            }
            current[value] = window;
        }

        previous.swap(current);
        previous_limit = current_limit;
    }

    cout << previous[target] << '\n';
}
