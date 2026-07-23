#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

// Sinh tất cả tổng của mọi tập con của mảng values.
// Bắt đầu với {0}, mỗi lần thêm 1 phần tử thì nhân đôi số tổng
// (mỗi tổng cũ tách thành: không lấy / có lấy phần tử này).
vector<long long> subset_sums(const vector<long long>& values) {
    vector<long long> sums{0};
    sums.reserve(1ULL << values.size());
    for (long long value : values) {
        const size_t current_size = sums.size();
        for (size_t i = 0; i < current_size; ++i) {
            sums.push_back(sums[i] + value);
        }
    }
    return sums;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc n quả táo và tính tổng trọng lượng total.
    int n;
    cin >> n;
    vector<long long> weights(n);
    long long total = 0;
    for (long long& weight : weights) {
        cin >> weight;
        total += weight;
    }

    // Meet in the middle: chia mảng thành nửa trái và nửa phải.
    const int half = n / 2;
    const vector<long long> left_values(weights.begin(), weights.begin() + half);
    const vector<long long> right_values(weights.begin() + half, weights.end());

    // Sinh mọi subset-sum của từng nửa; sắp xếp nửa phải để tìm nhị phân.
    const vector<long long> left_sums = subset_sums(left_values);
    vector<long long> right_sums = subset_sums(right_values);
    sort(right_sums.begin(), right_sums.end());

    // Với mỗi tổng a của nửa trái, cần chọn b ở nửa phải sao cho
    // |total - 2*(a+b)| nhỏ nhất, tức 2*b gần key = total - 2*a nhất.
    long long best = numeric_limits<long long>::max();
    for (long long left_sum : left_sums) {
        const long long key = total - 2 * left_sum;

        // Tìm vị trí đầu tiên có 2*right_sum >= key (tức b gần key/2 nhất).
        const auto position = lower_bound(
            right_sums.begin(), right_sums.end(), key,
            [](long long right_sum, long long target) {
                return 2 * right_sum < target;
            });

        // Chỉ cần thử 2 ứng viên liền kề quanh key/2 để lấy hiệu nhỏ nhất.
        if (position != right_sums.end()) {
            best = min(best, llabs(key - 2 * *position));
        }
        if (position != right_sums.begin()) {
            const auto previous = position - 1;
            best = min(best, llabs(key - 2 * *previous));
        }
    }

    cout << best << '\n';
    return 0;
}
