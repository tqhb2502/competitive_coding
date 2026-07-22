#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void generate_next_masks(int row, int height, int current_mask, int next_mask,
                         vector<int>& transitions) {
    if (row == height) {
        transitions.push_back(next_mask);
        return;
    }

    if ((current_mask & (1 << row)) != 0) {
        generate_next_masks(row + 1, height, current_mask, next_mask, transitions);
        return;
    }

    // A horizontal domino extends into the next column.
    generate_next_masks(row + 1, height, current_mask, next_mask | (1 << row), transitions);

    // A vertical domino fills two adjacent cells in this column.
    if (row + 1 < height && (current_mask & (1 << (row + 1))) == 0) {
        generate_next_masks(row + 2, height, current_mask, next_mask, transitions);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr int MOD = 1'000'000'007;

    int n, m;
    cin >> n >> m;
    if (n > m) {
        swap(n, m);
    }

    const int states = 1 << n;
    vector<vector<int>> transitions(states);
    for (int mask = 0; mask < states; ++mask) {
        generate_next_masks(0, n, mask, 0, transitions[mask]);
    }

    vector<int> dp(states, 0), next(states, 0);
    dp[0] = 1;
    for (int column = 0; column < m; ++column) {
        fill(next.begin(), next.end(), 0);
        for (int current_mask = 0; current_mask < states; ++current_mask) {
            if (dp[current_mask] == 0) {
                continue;
            }
            for (const int next_mask : transitions[current_mask]) {
                next[next_mask] += dp[current_mask];
                if (next[next_mask] >= MOD) {
                    next[next_mask] -= MOD;
                }
            }
        }
        dp.swap(next);
    }

    cout << dp[0] << '\n';
    return 0;
}
