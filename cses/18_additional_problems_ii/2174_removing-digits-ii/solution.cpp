#include <algorithm>
#include <array>
#include <iostream>

using namespace std;

struct Transition {
    long long steps = 0;
    int deficit = 0;
};

array<long long, 19> powerOfTen{};
Transition completeBlock[19][10][10];

Transition processSuffix(int digits, long long suffix, int prefixMaximum) {
    if (digits == 1) {
        if (suffix >= prefixMaximum) {
            return {2, prefixMaximum};
        }
        return {1, prefixMaximum - static_cast<int>(suffix)};
    }

    const long long blockSize = powerOfTen[digits - 1];
    int leadingDigit = static_cast<int>(suffix / blockSize);
    long long remainder = suffix % blockSize;
    long long totalSteps = 0;

    while (true) {
        const int contextMaximum = max(prefixMaximum, leadingDigit);
        Transition transition;
        if (remainder >= blockSize - 9) {
            const int initialDeficit =
                static_cast<int>(blockSize - remainder);
            transition = completeBlock[digits - 1][contextMaximum]
                                      [initialDeficit];
        } else {
            transition =
                processSuffix(digits - 1, remainder, contextMaximum);
        }
        totalSteps += transition.steps;
        if (leadingDigit == 0) {
            return {totalSteps, transition.deficit};
        }
        --leadingDigit;
        remainder = blockSize - transition.deficit;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    powerOfTen[0] = 1;
    for (int exponent = 1; exponent <= 18; ++exponent) {
        powerOfTen[exponent] = powerOfTen[exponent - 1] * 10;
    }
    for (int digits = 1; digits <= 18; ++digits) {
        for (int prefixMaximum = 1; prefixMaximum <= 9; ++prefixMaximum) {
            for (int deficit = 1; deficit <= 9; ++deficit) {
                completeBlock[digits][prefixMaximum][deficit] =
                    processSuffix(digits, powerOfTen[digits] - deficit,
                                  prefixMaximum);
            }
        }
    }

    long long number;
    cin >> number;
    long long answer = 0;
    while (number >= 10) {
        int exponent = 1;
        while (exponent < 18 && powerOfTen[exponent + 1] <= number) {
            ++exponent;
        }
        const long long blockSize = powerOfTen[exponent];
        const int leadingDigit = static_cast<int>(number / blockSize);
        const long long suffix = number % blockSize;
        const Transition transition =
            processSuffix(exponent, suffix, leadingDigit);
        answer += transition.steps;
        number = leadingDigit * blockSize - transition.deficit;
    }
    if (number != 0) {
        ++answer;
    }
    cout << answer << '\n';
}
