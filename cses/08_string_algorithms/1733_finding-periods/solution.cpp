#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;
    const int length = static_cast<int>(text.size());

    // KMP prefix function: prefix[i] = độ dài border dài nhất của tiền tố s[0..i]
    std::vector<int> prefix(length, 0);
    for (int index = 1; index < length; ++index) {
        int matched = prefix[index - 1];
        // Lùi theo border chain cho tới khi khớp được ký tự hoặc về 0
        while (matched > 0 && text[index] != text[matched]) {
            matched = prefix[matched - 1];
        }
        if (text[index] == text[matched]) {
            ++matched;
        }
        prefix[index] = matched;
    }

    // Đi dọc border chain: mỗi border b cho một period p = length - b
    // (theo thứ tự tăng dần vì border giảm dần)
    std::vector<int> periods;
    for (int border = prefix[length - 1]; border > 0;
         border = prefix[border - 1]) {
        periods.push_back(length - border);
    }
    // Border rỗng (b = 0) ứng với period bằng cả xâu, luôn hợp lệ
    periods.push_back(length);

    for (std::size_t index = 0; index < periods.size(); ++index) {
        if (index > 0) {
            std::cout << ' ';
        }
        std::cout << periods[index];
    }
    std::cout << '\n';
    return 0;
}
