#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    const int n = static_cast<int>(s.size());

    // Nhân đôi xâu để mọi rotation là một xâu con độ dài n bắt đầu tại 0..n-1,
    // tránh phải tính chỉ số modulo.
    const std::string doubled = s + s;

    // failure function (tương tự KMP); best là vị trí bắt đầu rotation nhỏ nhất.
    std::vector<int> failure(static_cast<std::size_t>(2 * n), -1);
    int best = 0;

    // Booth's algorithm: duyệt xâu nhân đôi, dùng failure để nhảy lùi khi lệch.
    for (int j = 1; j < 2 * n; ++j) {
        const char current = doubled[static_cast<std::size_t>(j)];
        int matched = failure[static_cast<std::size_t>(j - best - 1)];

        // So sánh ký tự hiện tại với ứng viên bắt đầu từ best; khi gặp ký tự
        // nhỏ hơn thì dời điểm bắt đầu best sang rotation tốt hơn.
        while (matched != -1 &&
               current != doubled[static_cast<std::size_t>(best + matched + 1)]) {
            if (current < doubled[static_cast<std::size_t>(best + matched + 1)]) {
                best = j - matched - 1;
            }
            matched = failure[static_cast<std::size_t>(matched)];
        }

        // Cập nhật failure function cho vị trí lệch so với điểm bắt đầu best.
        if (current != doubled[static_cast<std::size_t>(best + matched + 1)]) {
            if (current < doubled[static_cast<std::size_t>(best + matched + 1)]) {
                best = j;
            }
            failure[static_cast<std::size_t>(j - best)] = -1;
        } else {
            failure[static_cast<std::size_t>(j - best)] = matched + 1;
        }
    }

    // Rotation nhỏ nhất là đoạn độ dài n bắt đầu tại best.
    std::cout << doubled.substr(static_cast<std::size_t>(best),
                                static_cast<std::size_t>(n))
              << '\n';
    return 0;
}
