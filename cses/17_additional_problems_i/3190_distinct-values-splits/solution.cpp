#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

constexpr long long MOD = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> values(static_cast<size_t>(n));
    for (long long& value : values) {
        cin >> value;
    }

    // Nén tọa độ các giá trị bằng bản sao đã sắp xếp để tra chỉ số bằng lower_bound
    vector<long long> coordinates = values;
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
    // lastPosition[k]: vị trí xuất hiện gần nhất của giá trị nén thứ k (0 nếu chưa gặp)
    vector<int> lastPosition(coordinates.size(), 0);

    // prefixWays[i] = dp[0] + ... + dp[i], với dp[0] = 1
    vector<long long> prefixWays(static_cast<size_t>(n) + 1);
    prefixWays[0] = 1;
    // left: vị trí nhỏ nhất để đoạn [left, position] không có giá trị lặp
    int left = 1;

    for (int position = 1; position <= n; ++position) {
        long long value = values[static_cast<size_t>(position - 1)];
        size_t index = static_cast<size_t>(lower_bound(coordinates.begin(), coordinates.end(), value)
                                           - coordinates.begin());
        // Thu hẹp cửa sổ trượt qua lần xuất hiện trước của cùng giá trị
        left = max(left, lastPosition[index] + 1);
        lastPosition[index] = position;

        // dp[position] = tổng dp[left-1 .. position-1], lấy qua hiệu hai prefix sum
        long long ways = prefixWays[static_cast<size_t>(position - 1)];
        if (left >= 2) {
            ways -= prefixWays[static_cast<size_t>(left - 2)];
        }
        ways %= MOD;
        if (ways < 0) {
            ways += MOD;
        }
        // Cập nhật prefix sum của dp
        prefixWays[static_cast<size_t>(position)] =
            (prefixWays[static_cast<size_t>(position - 1)] + ways) % MOD;

        // Đáp án là dp[n]
        if (position == n) {
            cout << ways << '\n';
        }
    }
}
