#include <algorithm>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    long long value;
    std::cin >> value;
    long long current = value;
    long long answer = value;

    for (int i = 1; i < n; ++i) {
        std::cin >> value;
        current = std::max(value, current + value);
        answer = std::max(answer, current);
    }

    std::cout << answer << '\n';
    return 0;
}
