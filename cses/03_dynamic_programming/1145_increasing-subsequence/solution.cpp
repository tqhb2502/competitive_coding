#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // tails[i] is the smallest possible final value of a strictly increasing
    // subsequence of length i + 1 in the prefix processed so far.
    std::vector<int> tails;
    tails.reserve(n);

    for (int i = 0; i < n; ++i) {
        int value;
        std::cin >> value;

        const auto position =
            std::lower_bound(tails.begin(), tails.end(), value);
        if (position == tails.end()) {
            tails.push_back(value);
        } else {
            *position = value;
        }
    }

    std::cout << tails.size() << '\n';
    return 0;
}
