#include <iostream>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long long target;
    std::cin >> n >> target;

    std::unordered_map<long long, long long> prefix_count;
    prefix_count.max_load_factor(0.7F);
    prefix_count.reserve(static_cast<std::size_t>(2 * n + 1));
    prefix_count[0] = 1;

    long long prefix = 0;
    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        long long value;
        std::cin >> value;
        prefix += value;

        const auto it = prefix_count.find(prefix - target);
        if (it != prefix_count.end()) {
            answer += it->second;
        }
        ++prefix_count[prefix];
    }

    std::cout << answer << '\n';
    return 0;
}
