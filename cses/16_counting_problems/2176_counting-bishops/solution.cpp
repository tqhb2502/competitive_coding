#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr int64_t MOD = 1'000'000'007;

vector<int64_t> count_one_color(vector<int> diagonal_lengths, const int bishops) {
    sort(diagonal_lengths.begin(), diagonal_lengths.end());
    vector<int64_t> ways(bishops + 1, 0);
    ways[0] = 1;

    for (const int length : diagonal_lengths) {
        for (int placed = bishops; placed >= 1; --placed) {
            const int available = length - placed + 1;
            if (available > 0) {
                ways[placed] =
                    (ways[placed] + ways[placed - 1] * available) % MOD;
            }
        }
    }
    return ways;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int board_size;
    int bishop_count;
    cin >> board_size >> bishop_count;

    const int maximum_bishops = (board_size == 1) ? 1 : 2 * board_size - 2;
    if (bishop_count > maximum_bishops) {
        cout << 0 << '\n';
        return 0;
    }

    vector<int> diagonal_lengths[2];
    for (int diagonal_sum = 2; diagonal_sum <= 2 * board_size; ++diagonal_sum) {
        const int length = (diagonal_sum <= board_size + 1)
                               ? diagonal_sum - 1
                               : 2 * board_size + 1 - diagonal_sum;
        diagonal_lengths[diagonal_sum % 2].push_back(length);
    }

    const vector<int64_t> first =
        count_one_color(diagonal_lengths[0], bishop_count);
    const vector<int64_t> second =
        count_one_color(diagonal_lengths[1], bishop_count);

    int64_t answer = 0;
    for (int first_count = 0; first_count <= bishop_count; ++first_count) {
        answer += first[first_count] * second[bishop_count - first_count] % MOD;
        if (answer >= MOD) {
            answer -= MOD;
        }
    }

    cout << answer << '\n';
}
