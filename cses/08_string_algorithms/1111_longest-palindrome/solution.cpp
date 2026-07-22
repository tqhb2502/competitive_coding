#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;
    const int n = static_cast<int>(s.size());
    const int transformed_length = 2 * n + 1;

    // Layout: left guard, separator, s[0], separator, ..., s[n-1],
    // separator, right guard. Original letters have positive byte values.
    std::vector<int> transformed(static_cast<std::size_t>(transformed_length + 2), 0);
    transformed[0] = -1;
    transformed[static_cast<std::size_t>(transformed_length + 1)] = -2;
    for (int i = 0; i < n; ++i) {
        transformed[static_cast<std::size_t>(2 * i + 2)] =
            static_cast<unsigned char>(s[static_cast<std::size_t>(i)]);
    }

    std::vector<int> radius(static_cast<std::size_t>(transformed_length + 2), 0);
    int center = 0;
    int right = 0;
    int best_center = 1;
    int best_length = 0;

    for (int i = 1; i <= transformed_length; ++i) {
        if (i < right) {
            const int mirror = 2 * center - i;
            radius[static_cast<std::size_t>(i)] =
                std::min(right - i, radius[static_cast<std::size_t>(mirror)]);
        }

        while (transformed[static_cast<std::size_t>(i + radius[static_cast<std::size_t>(i)] + 1)] ==
               transformed[static_cast<std::size_t>(i - radius[static_cast<std::size_t>(i)] - 1)]) {
            ++radius[static_cast<std::size_t>(i)];
        }

        if (i + radius[static_cast<std::size_t>(i)] > right) {
            center = i;
            right = i + radius[static_cast<std::size_t>(i)];
        }
        if (radius[static_cast<std::size_t>(i)] > best_length) {
            best_length = radius[static_cast<std::size_t>(i)];
            best_center = i;
        }
    }

    const int start = (best_center - 1 - best_length) / 2;
    std::cout << s.substr(static_cast<std::size_t>(start),
                          static_cast<std::size_t>(best_length))
              << '\n';
    return 0;
}
