#include <iostream>
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

    long long answer = 0;
    long long current_sum = 0;
    int left = 0;

    for (int right = 0; right < n; ++right) {
        current_sum += values[right];
        while (current_sum > target) {
            current_sum -= values[left];
            ++left;
        }
        if (current_sum == target) {
            ++answer;
        }
    }

    std::cout << answer << '\n';
    return 0;
}
