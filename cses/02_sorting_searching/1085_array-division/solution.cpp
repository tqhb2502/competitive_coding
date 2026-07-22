#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int maximum_parts;
    std::cin >> n >> maximum_parts;

    std::vector<long long> values(n);
    for (long long& value : values) {
        std::cin >> value;
    }

    const auto feasible = [&](const long long limit) {
        int parts = 1;
        long long current_sum = 0;

        for (const long long value : values) {
            if (current_sum + value > limit) {
                ++parts;
                current_sum = value;
                if (parts > maximum_parts) {
                    return false;
                }
            } else {
                current_sum += value;
            }
        }
        return true;
    };

    long long low = *std::max_element(values.begin(), values.end());
    long long high = std::accumulate(values.begin(), values.end(), 0LL);

    while (low < high) {
        const long long middle = low + (high - low) / 2;
        if (feasible(middle)) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    std::cout << low << '\n';
    return 0;
}
