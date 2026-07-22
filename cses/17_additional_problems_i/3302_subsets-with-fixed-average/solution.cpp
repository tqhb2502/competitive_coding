#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

constexpr int MOD = 1'000'000'007;

std::vector<int> subsetSums(const std::vector<int>& values, int limit) {
    std::vector<int> ways(static_cast<std::size_t>(limit) + 1U, 0);
    ways[0] = 1;
    int currentMaximum = 0;
    for (const int value : values) {
        if (value > limit) {
            continue;
        }
        const int nextMaximum = std::min(limit, currentMaximum + value);
        for (int sum = nextMaximum; sum >= value; --sum) {
            int updated = ways[static_cast<std::size_t>(sum)] +
                          ways[static_cast<std::size_t>(sum - value)];
            if (updated >= MOD) {
                updated -= MOD;
            }
            ways[static_cast<std::size_t>(sum)] = updated;
        }
        currentMaximum = nextMaximum;
    }
    return ways;
}

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

    int n = 0;
    int targetAverage = 0;
    std::cin >> n >> targetAverage;
    std::vector<int> positive;
    std::vector<int> negative;
    int zeroCount = 0;
    int positiveSum = 0;
    int negativeSum = 0;

    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        const int difference = value - targetAverage;
        if (difference > 0) {
            positive.push_back(difference);
            positiveSum += difference;
        } else if (difference < 0) {
            negative.push_back(-difference);
            negativeSum -= difference;
        } else {
            ++zeroCount;
        }
    }

    const int limit = std::min(positiveSum, negativeSum);
    const std::vector<int> positiveWays = subsetSums(positive, limit);
    const std::vector<int> negativeWays = subsetSums(negative, limit);

    long long pairedWays = 0;
    for (int sum = 0; sum <= limit; ++sum) {
        pairedWays = (pairedWays +
                      static_cast<long long>(positiveWays[static_cast<std::size_t>(sum)]) *
                          negativeWays[static_cast<std::size_t>(sum)]) % MOD;
    }
    long long answer = pairedWays * powerOfTwo(zeroCount) % MOD;
    answer = (answer - 1 + MOD) % MOD;
    std::cout << answer << '\n';
    return 0;
}
