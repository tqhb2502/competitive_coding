#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n;
    std::cin >> n;

    while (true) {
        std::cout << n;
        if (n == 1) {
            break;
        }
        std::cout << ' ';
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
    }
    std::cout << '\n';

    return 0;
}
