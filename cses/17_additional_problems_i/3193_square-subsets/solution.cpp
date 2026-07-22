#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {

constexpr int LIMIT = 5000;
constexpr int MAX_PRIMES = 669;
constexpr int BLOCKS = (MAX_PRIMES + 63) / 64;
constexpr long long MOD = 1'000'000'007LL;
using Row = std::array<std::uint64_t, BLOCKS>;

long long powerOfTwo(int exponent) {
    long long result = 1;
    long long base = 2;
    while (exponent > 0) {
        if ((exponent & 1) != 0) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::array<int, LIMIT + 1> smallestPrime{};
    std::vector<int> primes;
    for (int value = 2; value <= LIMIT; ++value) {
        if (smallestPrime[static_cast<std::size_t>(value)] == 0) {
            smallestPrime[static_cast<std::size_t>(value)] = value;
            primes.push_back(value);
        }
        for (const int prime : primes) {
            if (prime > smallestPrime[static_cast<std::size_t>(value)] ||
                static_cast<long long>(value) * prime > LIMIT) {
                break;
            }
            smallestPrime[static_cast<std::size_t>(value * prime)] = prime;
        }
    }

    std::array<int, LIMIT + 1> primeIndex{};
    primeIndex.fill(-1);
    for (std::size_t i = 0; i < primes.size(); ++i) {
        primeIndex[static_cast<std::size_t>(primes[i])] = static_cast<int>(i);
    }

    int n = 0;
    std::cin >> n;
    std::array<Row, MAX_PRIMES> basis{};
    int rank = 0;

    for (int item = 0; item < n; ++item) {
        int value = 0;
        std::cin >> value;
        Row row{};
        while (value > 1) {
            const int prime = smallestPrime[static_cast<std::size_t>(value)];
            int parity = 0;
            while (value % prime == 0) {
                value /= prime;
                parity ^= 1;
            }
            if (parity != 0) {
                const int bit = primeIndex[static_cast<std::size_t>(prime)];
                row[static_cast<std::size_t>(bit / 64)] ^=
                    std::uint64_t{1} << static_cast<unsigned int>(bit % 64);
            }
        }

        for (int bit = static_cast<int>(primes.size()) - 1; bit >= 0; --bit) {
            const std::uint64_t mask =
                std::uint64_t{1} << static_cast<unsigned int>(bit % 64);
            if ((row[static_cast<std::size_t>(bit / 64)] & mask) == 0U) {
                continue;
            }
            Row& pivot = basis[static_cast<std::size_t>(bit)];
            if ((pivot[static_cast<std::size_t>(bit / 64)] & mask) == 0U) {
                pivot = row;
                ++rank;
                break;
            }
            for (int block = 0; block < BLOCKS; ++block) {
                row[static_cast<std::size_t>(block)] ^=
                    pivot[static_cast<std::size_t>(block)];
            }
        }
    }

    std::cout << powerOfTwo(n - rank) << '\n';
    return 0;
}
