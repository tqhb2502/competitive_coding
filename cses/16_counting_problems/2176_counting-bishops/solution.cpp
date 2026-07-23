#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr int64_t MOD = 1'000'000'007;

// Đếm số cách đặt quân tượng trên một màu, dựa vào độ dài các đường chéo.
// ways[j] = số cách đặt j quân tượng sau khi đã xử lý một số đường chéo.
vector<int64_t> count_one_color(vector<int> diagonal_lengths, const int bishops) {
    // Sắp xếp độ dài tăng dần để mỗi đường chéo đã bị chiếm cắt đường chéo
    // hiện tại tại đúng một ô hợp lệ.
    sort(diagonal_lengths.begin(), diagonal_lengths.end());
    vector<int64_t> ways(bishops + 1, 0);
    ways[0] = 1;

    for (const int length : diagonal_lengths) {
        // Cập nhật số quân đã đặt theo thứ tự giảm dần (DP knapsack).
        for (int placed = bishops; placed >= 1; --placed) {
            // Đặt quân thứ placed: placed-1 quân cũ đã chiếm placed-1 đường
            // chéo hướng còn lại, nên còn length-(placed-1) ô hợp lệ để chọn.
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

    // Số quân tượng tối đa không tấn công nhau: 1 nếu n=1, ngược lại 2n-2.
    const int maximum_bishops = (board_size == 1) ? 1 : 2 * board_size - 2;
    if (bishop_count > maximum_bishops) {
        cout << 0 << '\n';
        return 0;
    }

    // Gom độ dài các đường chéo theo tổng (hàng+cột); chẵn/lẻ tương ứng hai màu.
    vector<int> diagonal_lengths[2];
    for (int diagonal_sum = 2; diagonal_sum <= 2 * board_size; ++diagonal_sum) {
        const int length = (diagonal_sum <= board_size + 1)
                               ? diagonal_sum - 1
                               : 2 * board_size + 1 - diagonal_sum;
        diagonal_lengths[diagonal_sum % 2].push_back(length);
    }

    // Đếm độc lập trên từng màu (hai màu ô không tấn công lẫn nhau).
    const vector<int64_t> first =
        count_one_color(diagonal_lengths[0], bishop_count);
    const vector<int64_t> second =
        count_one_color(diagonal_lengths[1], bishop_count);

    // Chập hai kết quả: tổng first[i] * second[k-i].
    int64_t answer = 0;
    for (int first_count = 0; first_count <= bishop_count; ++first_count) {
        answer += first[first_count] * second[bishop_count - first_count] % MOD;
        if (answer >= MOD) {
            answer -= MOD;
        }
    }

    cout << answer << '\n';
}
