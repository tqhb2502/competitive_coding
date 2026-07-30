#include <algorithm>
#include <iostream>
#include <vector>

constexpr long long MOD = 1'000'000'007LL;

// Lũy thừa nhanh base^exponent theo modulo MOD
long long modular_power(long long base, int exponent) {
    long long result = 1;
    while (exponent > 0) {
        if ((exponent & 1) != 0) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent /= 2;
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> permutation(n + 1);
    for (int position = 1; position <= n; ++position) {
        std::cin >> permutation[position];
    }

    // Tách hoán vị thành các cycle: duyệt i -> p[i] và đánh dấu visited.
    // Đáp án là bậc của hoán vị = LCM độ dài các cycle.
    std::vector<int> cycle_lengths;
    std::vector<char> visited(n + 1, false);
    for (int start = 1; start <= n; ++start) {
        if (visited[start]) {
            continue;
        }
        int length = 0;
        int position = start;
        while (!visited[position]) {
            visited[position] = true;
            position = permutation[position];
            ++length;
        }
        cycle_lengths.push_back(length);
    }

    // SPF sieve (smallest prime factor) tới n để factorize nhanh mỗi độ dài cycle
    std::vector<int> smallest_prime_factor(n + 1, 0);
    for (int value = 2; value <= n; ++value) {
        if (smallest_prime_factor[value] != 0) {
            continue;
        }
        smallest_prime_factor[value] = value;
        if (value <= n / value) {
            for (int multiple = value * value; multiple <= n;
                 multiple += value) {
                if (smallest_prime_factor[multiple] == 0) {
                    smallest_prime_factor[multiple] = value;
                }
            }
        }
    }

    // LCM qua phân tích thừa số: với mỗi prime lấy số mũ lớn nhất
    // xuất hiện trên tất cả các độ dài cycle
    std::vector<int> maximum_exponent(n + 1, 0);
    for (int length : cycle_lengths) {
        while (length > 1) {
            const int prime = smallest_prime_factor[length];
            int exponent = 0;
            do {
                length /= prime;
                ++exponent;
            } while (length % prime == 0);
            maximum_exponent[prime] = std::max(
                maximum_exponent[prime], exponent
            );
        }
    }

    // LCM mod p = tích của prime^(số mũ lớn nhất) theo modulo MOD
    long long answer = 1;
    for (int prime = 2; prime <= n; ++prime) {
        if (maximum_exponent[prime] > 0) {
            answer = answer * modular_power(
                prime, maximum_exponent[prime]
            ) % MOD;
        }
    }
    std::cout << answer << '\n';
    return 0;
}
