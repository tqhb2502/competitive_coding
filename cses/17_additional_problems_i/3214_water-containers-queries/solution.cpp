#include <iostream>
#include <numeric>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int tests = 0;
    std::cin >> tests;
    while (tests-- > 0) {
        long long capacityA = 0;
        long long capacityB = 0;
        long long target = 0;
        std::cin >> capacityA >> capacityB >> target;
        const long long divisor = std::gcd(capacityA, capacityB);
        const bool possible = target <= capacityA && target % divisor == 0;
        std::cout << (possible ? "YES\n" : "NO\n");
    }
    return 0;
}
