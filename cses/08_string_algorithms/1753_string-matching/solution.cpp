#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::string pattern;
    std::cin >> text >> pattern;

    const int pattern_length = static_cast<int>(pattern.size());

    // Bước 1: tính failure function (prefix function) của pattern.
    // prefix[i] = độ dài biên (border) dài nhất của đoạn pattern[0..i].
    std::vector<int> prefix(pattern_length, 0);
    for (int index = 1; index < pattern_length; ++index) {
        int matched = prefix[index - 1];
        // Lùi theo failure function tới khi khớp hoặc về 0.
        while (matched > 0 && pattern[index] != pattern[matched]) {
            matched = prefix[matched - 1];
        }
        if (pattern[index] == pattern[matched]) {
            ++matched;
        }
        prefix[index] = matched;
    }

    // Bước 2: quét text, đếm số lần khớp đầy đủ (kể cả chồng lấn).
    int occurrences = 0;
    int matched = 0;  // độ dài phần pattern đang khớp hiện tại
    for (const char character : text) {
        // Khi mismatch, lùi con trỏ pattern về biên dài nhất còn khớp được.
        while (matched > 0 && character != pattern[matched]) {
            matched = prefix[matched - 1];
        }
        if (character == pattern[matched]) {
            ++matched;
        }
        if (matched == pattern_length) {
            // Tìm thấy một lần xuất hiện; lùi để đếm các lần chồng lấn tiếp theo.
            ++occurrences;
            matched = prefix[matched - 1];
        }
    }

    std::cout << occurrences << '\n';
    return 0;
}
