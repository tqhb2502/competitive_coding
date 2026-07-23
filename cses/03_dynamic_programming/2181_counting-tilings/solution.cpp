#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Liệt kê mọi cách lát đầy một cột: với current_mask là các ô đã bị domino ngang
// từ cột trước thò sang chiếm chỗ, sinh mọi next_mask (tập ô thò sang cột kế).
void generate_next_masks(int row, int height, int current_mask, int next_mask,
                         vector<int>& transitions) {
    if (row == height) {
        transitions.push_back(next_mask);
        return;
    }

    // Ô đã bị chiếm sẵn từ cột trước -> bỏ qua, không thò sang cột kế.
    if ((current_mask & (1 << row)) != 0) {
        generate_next_masks(row + 1, height, current_mask, next_mask, transitions);
        return;
    }

    // Lựa chọn 1: đặt domino ngang thò sang cột kế -> bật bit row trong next_mask.
    generate_next_masks(row + 1, height, current_mask, next_mask | (1 << row), transitions);

    // Lựa chọn 2: đặt domino dọc phủ (row, row+1) ngay trong cột này (không thò sang).
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
    // Đối xứng chuyển vị: lát (n x m) bằng lát (m x n). Chọn n nhỏ hơn để 2^n nhỏ nhất.
    if (n > m) {
        swap(n, m);
    }

    // Precompute các cạnh chuyển: transitions[mask] = danh sách next_mask sinh ra từ mask.
    const int states = 1 << n;
    vector<vector<int>> transitions(states);
    for (int mask = 0; mask < states; ++mask) {
        generate_next_masks(0, n, mask, 0, transitions[mask]);
    }

    // DP qua m cột. dp[mask] = số cách khi mask là tập ô bị thò sang cột đang xét.
    vector<int> dp(states, 0), next(states, 0);
    dp[0] = 1;  // trước cột đầu tiên: không có ô nào thò sang.
    for (int column = 0; column < m; ++column) {
        fill(next.begin(), next.end(), 0);
        for (int current_mask = 0; current_mask < states; ++current_mask) {
            if (dp[current_mask] == 0) {
                continue;
            }
            // Cộng dồn dp[current_mask] vào mọi next_mask hợp lệ (giữ trong khoảng [0, MOD)).
            for (const int next_mask : transitions[current_mask]) {
                next[next_mask] += dp[current_mask];
                if (next[next_mask] >= MOD) {
                    next[next_mask] -= MOD;
                }
            }
        }
        dp.swap(next);
    }

    // Sau m cột không được có ô nào thò ra ngoài lưới -> đáp án là dp[0].
    cout << dp[0] << '\n';
    return 0;
}
