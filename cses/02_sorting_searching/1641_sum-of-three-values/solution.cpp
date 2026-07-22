#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long target;
    cin >> n >> target;

    vector<pair<long long, int>> values(n);
    for (int i = 0; i < n; ++i) {
        cin >> values[i].first;
        values[i].second = i + 1;
    }

    sort(values.begin(), values.end());

    if (n >= 3) {
        const long long largest_pair = values[n - 1].first + values[n - 2].first;

        for (int i = 0; i + 2 < n; ++i) {
            const long long needed_pair = target - values[i].first;
            if (needed_pair > largest_pair) {
                continue;
            }
            if (needed_pair < values[i + 1].first + values[i + 2].first) {
                break;
            }

            int left = i + 1;
            int right = n - 1;
            while (left < right) {
                const long long pair_sum = values[left].first + values[right].first;
                if (pair_sum == needed_pair) {
                    cout << values[i].second << ' ' << values[left].second << ' '
                         << values[right].second << '\n';
                    return 0;
                }
                if (pair_sum < needed_pair) {
                    ++left;
                } else {
                    --right;
                }
            }
        }
    }

    cout << "IMPOSSIBLE\n";
    return 0;
}
