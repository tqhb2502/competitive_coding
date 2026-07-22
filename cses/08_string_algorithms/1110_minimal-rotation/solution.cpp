#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    const int n = static_cast<int>(s.size());
    const std::string doubled = s + s;
    std::vector<int> failure(static_cast<std::size_t>(2 * n), -1);
    int best = 0;

    for (int j = 1; j < 2 * n; ++j) {
        const char current = doubled[static_cast<std::size_t>(j)];
        int matched = failure[static_cast<std::size_t>(j - best - 1)];

        while (matched != -1 &&
               current != doubled[static_cast<std::size_t>(best + matched + 1)]) {
            if (current < doubled[static_cast<std::size_t>(best + matched + 1)]) {
                best = j - matched - 1;
            }
            matched = failure[static_cast<std::size_t>(matched)];
        }

        if (current != doubled[static_cast<std::size_t>(best + matched + 1)]) {
            if (current < doubled[static_cast<std::size_t>(best + matched + 1)]) {
                best = j;
            }
            failure[static_cast<std::size_t>(j - best)] = -1;
        } else {
            failure[static_cast<std::size_t>(j - best)] = matched + 1;
        }
    }

    std::cout << doubled.substr(static_cast<std::size_t>(best),
                                static_cast<std::size_t>(n))
              << '\n';
    return 0;
}
