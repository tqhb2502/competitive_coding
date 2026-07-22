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
    vector<long long> coordinates = values;
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
    vector<int> lastPosition(coordinates.size(), 0);

    vector<long long> prefixWays(static_cast<size_t>(n) + 1);
    prefixWays[0] = 1;
    int left = 1;

    for (int position = 1; position <= n; ++position) {
        long long value = values[static_cast<size_t>(position - 1)];
        size_t index = static_cast<size_t>(lower_bound(coordinates.begin(), coordinates.end(), value)
                                           - coordinates.begin());
        left = max(left, lastPosition[index] + 1);
        lastPosition[index] = position;

        long long ways = prefixWays[static_cast<size_t>(position - 1)];
        if (left >= 2) {
            ways -= prefixWays[static_cast<size_t>(left - 2)];
        }
        ways %= MOD;
        if (ways < 0) {
            ways += MOD;
        }
        prefixWays[static_cast<size_t>(position)] =
            (prefixWays[static_cast<size_t>(position - 1)] + ways) % MOD;

        if (position == n) {
            cout << ways << '\n';
        }
    }
}
