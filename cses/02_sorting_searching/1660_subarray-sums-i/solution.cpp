#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc n và tổng mục tiêu target (dùng long long vì tổng có thể tới 2*10^14)
    int n;
    long long target;
    std::cin >> n >> target;

    std::vector<long long> values(n);
    for (long long& value : values) {
        std::cin >> value;
    }

    long long answer = 0;
    long long current_sum = 0;  // tổng của cửa sổ [left, right]
    int left = 0;

    // Two-pointer: mở rộng right, cộng dồn vào current_sum
    for (int right = 0; right < n; ++right) {
        current_sum += values[right];
        // Thu hẹp cửa sổ từ bên trái khi tổng vượt quá target
        while (current_sum > target) {
            current_sum -= values[left];
            ++left;
        }
        // Cửa sổ hiện tại có tổng đúng bằng target => đếm thêm một subarray
        if (current_sum == target) {
            ++answer;
        }
    }

    std::cout << answer << '\n';
    return 0;
}
