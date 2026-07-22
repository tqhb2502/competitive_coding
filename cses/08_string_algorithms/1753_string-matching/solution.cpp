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
    std::vector<int> prefix(pattern_length, 0);
    for (int index = 1; index < pattern_length; ++index) {
        int matched = prefix[index - 1];
        while (matched > 0 && pattern[index] != pattern[matched]) {
            matched = prefix[matched - 1];
        }
        if (pattern[index] == pattern[matched]) {
            ++matched;
        }
        prefix[index] = matched;
    }

    int occurrences = 0;
    int matched = 0;
    for (const char character : text) {
        while (matched > 0 && character != pattern[matched]) {
            matched = prefix[matched - 1];
        }
        if (character == pattern[matched]) {
            ++matched;
        }
        if (matched == pattern_length) {
            ++occurrences;
            matched = prefix[matched - 1];
        }
    }

    std::cout << occurrences << '\n';
    return 0;
}
