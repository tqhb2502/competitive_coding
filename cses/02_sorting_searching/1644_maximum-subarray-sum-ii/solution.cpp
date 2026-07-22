#include <algorithm>
#include <deque>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int min_length;
    int max_length;
    std::cin >> n >> min_length >> max_length;

    std::vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        long long value;
        std::cin >> value;
        prefix[i] = prefix[i - 1] + value;
    }

    std::deque<int> candidates;
    long long answer = std::numeric_limits<long long>::lowest();

    for (int right = min_length; right <= n; ++right) {
        const int added = right - min_length;
        while (!candidates.empty() && prefix[candidates.back()] >= prefix[added]) {
            candidates.pop_back();
        }
        candidates.push_back(added);

        const int first_valid = right - max_length;
        while (candidates.front() < first_valid) {
            candidates.pop_front();
        }

        answer = std::max(answer, prefix[right] - prefix[candidates.front()]);
    }

    std::cout << answer << '\n';
    return 0;
}
