#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x;
    cin >> n >> x;

    vector<long long> weight(n);
    for (long long& value : weight) {
        cin >> value;
    }

    // Mã hoá cặp (rides, last_weight) thành một số: rides * BIG + last_weight.
    // BIG = x + 1 vì cân nặng chuyến cuối luôn <= x, nên lấy min trên số này
    // chính là so sánh lexicographic (ưu tiên rides nhỏ, rồi tới last_weight).
    const long long BIG = x + 1;
    const int size = 1 << n;
    vector<long long> dp(size);
    dp[0] = BIG;  // Base case: đã có 1 chuyến mở, chưa chở ai, cân nặng 0.

    for (int mask = 1; mask < size; ++mask) {
        long long best = numeric_limits<long long>::max();
        int remaining = mask;
        // Thử chọn từng người person là người được thêm cuối cùng vào mask.
        while (remaining != 0) {
            const int bit = remaining & -remaining;
            const int person = __builtin_ctz(static_cast<unsigned>(bit));
            const long long previous = dp[mask ^ bit];
            const long long last_weight = previous % BIG;

            long long candidate;
            if (last_weight + weight[person] <= x) {
                // Còn chỗ trong chuyến đang mở -> nhét person vào chuyến đó.
                candidate = previous + weight[person];
            } else {
                // Chuyến cuối đã đầy -> mở chuyến mới chỉ chứa person.
                candidate = previous - last_weight + BIG + weight[person];
            }
            best = min(best, candidate);
            remaining ^= bit;
        }
        dp[mask] = best;
    }

    // Đáp án là phần rides của state đầy đủ.
    cout << dp[size - 1] / BIG << '\n';
    return 0;
}
