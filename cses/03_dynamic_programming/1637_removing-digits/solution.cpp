#include <algorithm>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Follow the greedy observation from the original solution: subtracting
    // the largest available digit is an optimal transition at every step.
    int steps = 0;
    while (n > 0) {
        int largest_digit = 0;
        for (int value = n; value > 0; value /= 10) {
            largest_digit = std::max(largest_digit, value % 10);
        }
        n -= largest_digit;
        ++steps;
    }

    std::cout << steps << '\n';
    return 0;
}
