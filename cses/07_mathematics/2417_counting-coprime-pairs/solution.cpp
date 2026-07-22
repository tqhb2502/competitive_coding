#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> values(n);
    int maximum_value = 0;
    for (int& value : values) {
        std::cin >> value;
        maximum_value = std::max(maximum_value, value);
    }

    std::vector<int> frequency(maximum_value + 1, 0);
    for (const int value : values) {
        ++frequency[value];
    }

    std::vector<int> mobius(maximum_value + 1, 0);
    std::vector<int> primes;
    std::vector<char> composite(maximum_value + 1, false);
    if (maximum_value >= 1) {
        mobius[1] = 1;
    }
    for (int value = 2; value <= maximum_value; ++value) {
        if (!composite[value]) {
            primes.push_back(value);
            mobius[value] = -1;
        }
        for (const int prime : primes) {
            if (prime > maximum_value / value) {
                break;
            }
            const int product = prime * value;
            composite[product] = true;
            if (value % prime == 0) {
                mobius[product] = 0;
                break;
            }
            mobius[product] = -mobius[value];
        }
    }

    long long answer = 0;
    for (int divisor = 1; divisor <= maximum_value; ++divisor) {
        if (mobius[divisor] == 0) {
            continue;
        }
        long long divisible_count = 0;
        for (int multiple = divisor; multiple <= maximum_value;
             multiple += divisor) {
            divisible_count += frequency[multiple];
        }
        answer += static_cast<long long>(mobius[divisor]) *
                  divisible_count * (divisible_count - 1) / 2;
    }

    std::cout << answer << '\n';
    return 0;
}
