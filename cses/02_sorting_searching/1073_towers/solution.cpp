#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> tops;
    tops.reserve(n);

    for (int i = 0; i < n; ++i) {
        int cube;
        std::cin >> cube;

        // Use the tower with the smallest top that is strictly larger.
        const auto it = std::upper_bound(tops.begin(), tops.end(), cube);
        if (it == tops.end()) {
            tops.push_back(cube);
        } else {
            *it = cube;
        }
    }

    std::cout << tops.size() << '\n';
    return 0;
}
