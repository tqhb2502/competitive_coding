#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // n: số phần tử; maximum_parts: số đoạn tối đa được phép chia (k)
    int n;
    int maximum_parts;
    std::cin >> n >> maximum_parts;

    std::vector<long long> values(n);
    for (long long& value : values) {
        std::cin >> value;
    }

    // feasible(limit): dùng greedy để kiểm tra có chia được mảng thành <= k đoạn
    // liên tiếp, mỗi đoạn có tổng <= limit hay không.
    const auto feasible = [&](const long long limit) {
        int parts = 1;
        long long current_sum = 0;

        for (const long long value : values) {
            // Nếu thêm phần tử làm vượt limit thì đóng đoạn cũ, mở đoạn mới
            if (current_sum + value > limit) {
                ++parts;
                current_sum = value;
                if (parts > maximum_parts) {
                    return false;
                }
            } else {
                current_sum += value;
            }
        }
        return true;
    };

    // Cận dưới = phần tử lớn nhất (mỗi đoạn phải chứa được ít nhất một phần tử)
    // Cận trên = tổng cả mảng (trường hợp k = 1)
    long long low = *std::max_element(values.begin(), values.end());
    long long high = std::accumulate(values.begin(), values.end(), 0LL);

    // Binary search trên đáp án: tìm limit khả thi nhỏ nhất
    while (low < high) {
        const long long middle = low + (high - low) / 2;
        if (feasible(middle)) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    std::cout << low << '\n';
    return 0;
}
