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

    // Encode (rides, last_weight) as rides * BIG + last_weight.
    const long long BIG = x + 1;
    const int size = 1 << n;
    vector<long long> dp(size);
    dp[0] = BIG;  // One open, initially empty ride.

    for (int mask = 1; mask < size; ++mask) {
        long long best = numeric_limits<long long>::max();
        int remaining = mask;
        while (remaining != 0) {
            const int bit = remaining & -remaining;
            const int person = __builtin_ctz(static_cast<unsigned>(bit));
            const long long previous = dp[mask ^ bit];
            const long long last_weight = previous % BIG;

            long long candidate;
            if (last_weight + weight[person] <= x) {
                candidate = previous + weight[person];
            } else {
                candidate = previous - last_weight + BIG + weight[person];
            }
            best = min(best, candidate);
            remaining ^= bit;
        }
        dp[mask] = best;
    }

    cout << dp[size - 1] / BIG << '\n';
    return 0;
}
