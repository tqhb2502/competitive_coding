#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<long long> values(n);
    std::vector<int> increasing_stack;
    increasing_stack.reserve(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> values[i];

        while (!increasing_stack.empty() &&
               values[increasing_stack.back()] >= values[i]) {
            increasing_stack.pop_back();
        }

        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << (increasing_stack.empty() ? 0 : increasing_stack.back() + 1);
        increasing_stack.push_back(i);
    }
    std::cout << '\n';

    return 0;
}
