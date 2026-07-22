#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long long target;
    std::cin >> n >> target;

    std::vector<long long> values(n);
    for (long long& value : values) {
        std::cin >> value;
    }

    std::unordered_map<long long, std::pair<int, int>> seen;
    seen.max_load_factor(0.7F);
    seen.reserve(static_cast<std::size_t>(n) * static_cast<std::size_t>(n) / 2);

    for (int i = 0; i < n; ++i) {
        // Every pair already in seen ends before i, so all four indices differ.
        for (int j = i + 1; j < n; ++j) {
            const long long needed = target - values[i] - values[j];
            const auto it = seen.find(needed);
            if (it != seen.end()) {
                std::cout << it->second.first + 1 << ' '
                          << it->second.second + 1 << ' '
                          << i + 1 << ' ' << j + 1 << '\n';
                return 0;
            }
        }

        // Insert only after searching, preserving k < i < next high pair.
        for (int k = 0; k < i; ++k) {
            seen.try_emplace(values[k] + values[i], k, i);
        }
    }

    std::cout << "IMPOSSIBLE\n";
    return 0;
}
