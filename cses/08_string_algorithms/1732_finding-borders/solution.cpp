#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;
    const int length = static_cast<int>(text.size());

    std::vector<int> prefix(length, 0);
    for (int index = 1; index < length; ++index) {
        int matched = prefix[index - 1];
        while (matched > 0 && text[index] != text[matched]) {
            matched = prefix[matched - 1];
        }
        if (text[index] == text[matched]) {
            ++matched;
        }
        prefix[index] = matched;
    }

    std::vector<int> borders;
    for (int border = prefix[length - 1]; border > 0;
         border = prefix[border - 1]) {
        borders.push_back(border);
    }
    std::reverse(borders.begin(), borders.end());

    for (std::size_t index = 0; index < borders.size(); ++index) {
        if (index > 0) {
            std::cout << ' ';
        }
        std::cout << borders[index];
    }
    std::cout << '\n';
    return 0;
}
