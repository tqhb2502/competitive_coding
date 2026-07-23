#include <algorithm>
#include <deque>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int min_length;  // a: độ dài nhỏ nhất của subarray
    int max_length;  // b: độ dài lớn nhất của subarray
    std::cin >> n >> min_length >> max_length;

    // prefix[i] = tổng arr[0..i-1]; tổng subarray (l+1..r) = prefix[r] - prefix[l]
    std::vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        long long value;
        std::cin >> value;
        prefix[i] = prefix[i - 1] + value;
    }

    // Monotonic deque giữ các chỉ số l theo prefix tăng dần (sliding window minimum)
    std::deque<int> candidates;
    long long answer = std::numeric_limits<long long>::lowest();

    for (int right = min_length; right <= n; ++right) {
        // Chỉ số l = right - a vừa trở nên hợp lệ, đẩy vào deque giữ tính đơn điệu
        const int added = right - min_length;
        while (!candidates.empty() && prefix[candidates.back()] >= prefix[added]) {
            candidates.pop_back();
        }
        candidates.push_back(added);

        // Loại các chỉ số quá cũ (< right - b) vì độ dài vượt quá b
        const int first_valid = right - max_length;
        while (candidates.front() < first_valid) {
            candidates.pop_front();
        }

        // Đầu deque là l có prefix nhỏ nhất trong cửa sổ [right-b, right-a]
        answer = std::max(answer, prefix[right] - prefix[candidates.front()]);
    }

    std::cout << answer << '\n';
    return 0;
}
