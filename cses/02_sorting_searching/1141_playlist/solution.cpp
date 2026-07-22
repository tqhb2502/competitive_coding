#include <algorithm>
#include <iostream>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::unordered_map<int, int> last_position;
    last_position.reserve(static_cast<std::size_t>(n) * 2);

    int left = 0;
    int best = 0;

    for (int right = 0; right < n; ++right) {
        int song;
        std::cin >> song;

        const auto it = last_position.find(song);
        if (it != last_position.end() && it->second >= left) {
            left = it->second + 1;
        }
        last_position[song] = right;
        best = std::max(best, right - left + 1);
    }

    std::cout << best << '\n';
    return 0;
}
