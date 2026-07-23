#include <algorithm>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Subarray phải khác rỗng nên khởi tạo bằng phần tử đầu tiên.
    long long value;
    std::cin >> value;
    long long current = value;  // tổng subarray tốt nhất kết thúc tại phần tử hiện tại
    long long answer = value;   // đáp án tốt nhất từng thấy

    // Kadane: nối vào đoạn trước khi có lợi, ngược lại bắt đầu lại từ value.
    for (int i = 1; i < n; ++i) {
        std::cin >> value;
        current = std::max(value, current + value);
        answer = std::max(answer, current);
    }

    std::cout << answer << '\n';
    return 0;
}
