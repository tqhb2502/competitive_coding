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

    const int maximum_inversions = n * (n - 1) / 2;
    const int target = min(inversions, maximum_inversions - inversions);

    vector<int> previous(target + 1, 0);
    vector<int> current(target + 1, 0);
    previous[0] = 1;
    int previous_limit = 0;

    for (int length = 2; length <= n; ++length) {
        const int current_limit = min(target, length * (length - 1) / 2);
        int window = 0;

        for (int value = 0; value <= current_limit; ++value) {
            if (value <= previous_limit) {
                window += previous[value];
                if (window >= MOD) {
                    window -= MOD;
                }
            }
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
