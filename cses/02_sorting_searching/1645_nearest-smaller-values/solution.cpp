#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<long long> values(n);
    // Stack lưu các CHỈ SỐ có giá trị tăng nghiêm ngặt từ đáy lên đỉnh
    std::vector<int> increasing_stack;
    increasing_stack.reserve(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> values[i];

        // Pop mọi chỉ số ở đỉnh có giá trị >= giá trị hiện tại: chúng không
        // thể là đáp án cho i cũng như cho bất kỳ vị trí nào bên phải i
        while (!increasing_stack.empty() &&
               values[increasing_stack.back()] >= values[i]) {
            increasing_stack.pop_back();
        }

        if (i > 0) {
            std::cout << ' ';
        }
        // Stack rỗng -> không có phần tử nhỏ hơn bên trái (in 0); ngược lại
        // đỉnh stack là chỉ số gần nhất bên trái nhỏ hơn (đổi sang 1-indexed)
        std::cout << (increasing_stack.empty() ? 0 : increasing_stack.back() + 1);
        increasing_stack.push_back(i);
    }
    std::cout << '\n';

    return 0;
}
